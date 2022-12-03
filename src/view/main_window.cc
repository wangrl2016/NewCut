//
// Created by wangrl2016 on 2022/11/30.
//

#include <glog/logging.h>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include "src/view/diagram_item.h"
#include "src/view/diagram_scene.h"
#include "src/view/diagram_text_item.h"
#include "src/view/main_window.h"

namespace nc {
    const int InsertTextButton = 10;

    MainWindow::MainWindow() {
        createActions();
        createToolBox();
        createMenus();

        scene = new DiagramScene(itemMenu, this);
        scene->setSceneRect(QRectF(0, 0, 5000, 5000));
        connect(scene, &DiagramScene::itemInserted,
                this, &MainWindow::itemInserted);
        connect(scene, &DiagramScene::textInserted,
                this, &MainWindow::textInserted);
        connect(scene, &DiagramScene::itemSelected,
                this, &MainWindow::itemSelected);

        createToolBars();

        QHBoxLayout* layout = new QHBoxLayout;
        layout->addWidget(toolBox);
        view = new QGraphicsView(scene);
        layout->addWidget(view);

        QWidget* widget = new QWidget;
        widget->setLayout(layout);

        setCentralWidget(widget);
        setWindowTitle(tr("Diagram Scene"));
        setUnifiedTitleAndToolBarOnMac(true);
    }

    MainWindow::~MainWindow() {
        LOG(INFO) << __FUNCTION__;
    }

    // slot实现
    void MainWindow::backgroundButtonGroupClicked(QAbstractButton* button) {
        const QList<QAbstractButton*> buttons = backgroundButtonGroup->buttons();
        for (QAbstractButton* myButton: buttons) {
            if (myButton != button)
                button->setChecked(false);
        }
        QString text = button->text();
        if (text == tr("Blue Grid"))
            scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
        else if (text == tr("White Grid"))
            scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
        else if (text == tr("Gray Grid"))
            scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
        else
            scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

        scene->update();
        view->update();
    }

    void MainWindow::buttonGroupClicked(QAbstractButton* button) {
        const QList<QAbstractButton*> buttons = buttonGroup->buttons();
        for (QAbstractButton* myButton: buttons) {
            if (myButton != button)
                button->setChecked(false);
        }
        const int id = buttonGroup->id(button);
        if (id == InsertTextButton) {
            scene->setMode(DiagramScene::InsertText);
        } else {
            scene->setItemType(DiagramItem::DiagramType(id));
            scene->setMode(DiagramScene::InsertItem);
        }
    }

    void MainWindow::deleteItem() {
        QList<QGraphicsItem*> selectedItems = scene->selectedItems();
        for (QGraphicsItem* item: qAsConst(selectedItems)) {
            if (item->type() == Arrow::Type) {
                scene->removeItem(item);
                Arrow* arrow = qgraphicsitem_cast<Arrow*>(item);
                arrow->startItem()->removeArrow(arrow);
                arrow->endItem()->removeArrow(arrow);
                delete item;
            }
        }

        selectedItems = scene->selectedItems();
        for (QGraphicsItem* item: qAsConst(selectedItems)) {
            if (item->type() == DiagramItem::Type)
                qgraphicsitem_cast<DiagramItem*>(item)->removeArrows();
            scene->removeItem(item);
            delete item;
        }
    }

    void MainWindow::pointerGroupClicked() {
        scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    }

    void MainWindow::bringToFront() {
        if (scene->selectedItems().isEmpty())
            return;

        QGraphicsItem* selectedItem = scene->selectedItems().first();
        const QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

        qreal zValue = 0;
        for (const QGraphicsItem* item: overlapItems) {
            if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
                zValue = item->zValue() + 0.1;
        }
        selectedItem->setZValue(zValue);
    }

    void MainWindow::sendToBack() {
        if (scene->selectedItems().isEmpty())
            return;

        QGraphicsItem* selectedItem = scene->selectedItems().first();
        const QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

        qreal zValue = 0;
        for (const QGraphicsItem* item: overlapItems) {
            if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
                zValue = item->zValue() - 0.1;
        }
        selectedItem->setZValue(zValue);
    }

    void MainWindow::itemInserted(DiagramItem* item) {
        pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
        scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
        buttonGroup->button(int(item->diagramType()))->setChecked(false);
    }

    void MainWindow::textInserted(QGraphicsTextItem* item) {
        buttonGroup->button(InsertTextButton)->setChecked(false);
        scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    }

    void MainWindow::currentFontChanged(const QFont& font) {
        handleFontChange();
    }

    void MainWindow::fontSizeChanged(const QString& size) {
        handleFontChange();
    }

    void MainWindow::sceneScaleChanged(const QString& scale) {
        double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
        QTransform oldMatrix = view->transform();
        view->resetTransform();
        view->translate(oldMatrix.dx(), oldMatrix.dy());
        view->scale(newScale, newScale);
    }

    void MainWindow::textColorChanged() {
        textAction = qobject_cast<QAction*>(sender());
        fontColorToolButton->setIcon(createColorToolButtonIcon(
                ":/images/textpointer.png",
                qvariant_cast<QColor>(textAction->data())));
        textButtonTriggered();
    }

    void MainWindow::itemColorChanged() {
        fillAction = qobject_cast<QAction*>(sender());
        fillColorToolButton->setIcon(createColorToolButtonIcon(
                ":/images/floodfill.png",
                qvariant_cast<QColor>(fillAction->data())));
        fillButtonTriggered();
    }

    void MainWindow::lineColorChanged() {
        lineAction = qobject_cast<QAction*>(sender());
        lineColorToolButton->setIcon(createColorToolButtonIcon(
                ":/images/linecolor.png",
                qvariant_cast<QColor>(lineAction->data())));
        lineButtonTriggered();
    }

    void MainWindow::textButtonTriggered() {
        scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
    }

    void MainWindow::fillButtonTriggered() {
        scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
    }

    void MainWindow::lineButtonTriggered() {
        scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
    }

    void MainWindow::handleFontChange() {
        QFont font = fontCombo->currentFont();
        font.setPointSize(fontSizeCombo->currentText().toInt());
        font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
        font.setItalic(italicAction->isChecked());
        font.setUnderline(underlineAction->isChecked());

        scene->setFont(font);
    }

    void MainWindow::itemSelected(QGraphicsItem* item) {
        DiagramTextItem* textItem =
                qgraphicsitem_cast<DiagramTextItem*>(item);

        QFont font = textItem->font();
        fontCombo->setCurrentFont(font);
        fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
        boldAction->setChecked(font.weight() == QFont::Bold);
        italicAction->setChecked(font.italic());
        underlineAction->setChecked(font.underline());
    }

    void MainWindow::about() {
        QMessageBox::about(this, tr("About Diagram Scene"),
                           tr("The <b>Diagram Scene</b> example shows "
                              "use of the graphics framework."));
    }

    void MainWindow::createToolBox() {
        buttonGroup = new QButtonGroup(this);
        buttonGroup->setExclusive(false);
        connect(buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                this, &MainWindow::buttonGroupClicked);
        QGridLayout* layout = new QGridLayout;
        layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 0);
        layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step), 0, 1);
        layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 1, 0);

        QToolButton* textButton = new QToolButton;
        textButton->setCheckable(true);
        buttonGroup->addButton(textButton, InsertTextButton);
        textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
        textButton->setIconSize(QSize(50, 50));
        QGridLayout* textLayout = new QGridLayout;
        textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
        textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
        QWidget* textWidget = new QWidget;
        textWidget->setLayout(textLayout);
        layout->addWidget(textWidget, 1, 1);

        layout->setRowStretch(3, 10);
        layout->setColumnStretch(2, 10);

        QWidget* itemWidget = new QWidget;
        itemWidget->setLayout(layout);

        backgroundButtonGroup = new QButtonGroup(this);
        connect(backgroundButtonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                this, &MainWindow::backgroundButtonGroupClicked);

        QGridLayout* backgroundLayout = new QGridLayout;
        backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                                                               ":/images/background1.png"), 0, 0);
        backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                                                               ":/images/background2.png"), 0, 1);
        backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                                                               ":/images/background3.png"), 1, 0);
        backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                                                               ":/images/background4.png"), 1, 1);

        backgroundLayout->setRowStretch(2, 10);
        backgroundLayout->setColumnStretch(2, 10);

        QWidget* backgroundWidget = new QWidget;
        backgroundWidget->setLayout(backgroundLayout);

        toolBox = new QToolBox;
        toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
        toolBox->setMinimumWidth(itemWidget->sizeHint().width());
        toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
        toolBox->addItem(backgroundWidget, tr("Backgrounds"));
    }

    void MainWindow::createActions() {
        toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                    tr("Bring to &Front"), this);
        toFrontAction->setShortcut(tr("Ctrl+F"));
        toFrontAction->setStatusTip(tr("Bring item to front"));
        connect(toFrontAction, &QAction::triggered, this, &MainWindow::bringToFront);
//! [23]

        sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
        sendBackAction->setShortcut(tr("Ctrl+T"));
        sendBackAction->setStatusTip(tr("Send item to back"));
        connect(sendBackAction, &QAction::triggered, this, &MainWindow::sendToBack);

        deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
        deleteAction->setShortcut(tr("Delete"));
        deleteAction->setStatusTip(tr("Delete item from diagram"));
        connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);

        exitAction = new QAction(tr("Abort"), this);
        exitAction->setShortcuts(QKeySequence::Quit);
        exitAction->setStatusTip(tr("Quit Scenediagram example"));
        connect(exitAction, &QAction::triggered, this, &QWidget::close);

        boldAction = new QAction(tr("Bold"), this);
        boldAction->setCheckable(true);
        QPixmap pixmap(":/images/bold.png");
        boldAction->setIcon(QIcon(pixmap));
        boldAction->setShortcut(tr("Ctrl+B"));
        connect(boldAction, &QAction::triggered, this, &MainWindow::handleFontChange);

        italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
        italicAction->setCheckable(true);
        italicAction->setShortcut(tr("Ctrl+I"));
        connect(italicAction, &QAction::triggered, this, &MainWindow::handleFontChange);

        underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
        underlineAction->setCheckable(true);
        underlineAction->setShortcut(tr("Ctrl+U"));
        connect(underlineAction, &QAction::triggered, this, &MainWindow::handleFontChange);

        aboutAction = new QAction(tr("Refer"), this);
        aboutAction->setShortcut(tr("F1"));
        connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
    }

    void MainWindow::createMenus() {
        fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(exitAction);

        itemMenu = menuBar()->addMenu(tr("&Item"));
        itemMenu->addAction(deleteAction);
        itemMenu->addSeparator();
        itemMenu->addAction(toFrontAction);
        itemMenu->addAction(sendBackAction);

        aboutMenu = menuBar()->addMenu(tr("&About"));
        aboutMenu->addAction(aboutAction);
    }

    void MainWindow::createToolBars() {
        editToolBar = addToolBar(tr("Edit"));
        editToolBar->addAction(deleteAction);
        editToolBar->addAction(toFrontAction);
        editToolBar->addAction(sendBackAction);

        fontCombo = new QFontComboBox();
        connect(fontCombo, &QFontComboBox::currentFontChanged,
                this, &MainWindow::currentFontChanged);

        fontSizeCombo = new QComboBox;
        fontSizeCombo->setEditable(true);
        for (int i = 8; i < 30; i = i + 2)
            fontSizeCombo->addItem(QString().setNum(i));
        QIntValidator* validator = new QIntValidator(2, 64, this);
        fontSizeCombo->setValidator(validator);
        connect(fontSizeCombo, &QComboBox::currentTextChanged,
                this, &MainWindow::fontSizeChanged);

        fontColorToolButton = new QToolButton;
        fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
        textAction = fontColorToolButton->menu()->defaultAction();
        fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
        fontColorToolButton->setAutoFillBackground(true);
        connect(fontColorToolButton, &QAbstractButton::clicked,
                this, &MainWindow::textButtonTriggered);

        fillColorToolButton = new QToolButton;
        fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
        fillAction = fillColorToolButton->menu()->defaultAction();
        fillColorToolButton->setIcon(createColorToolButtonIcon(
                ":/images/floodfill.png", Qt::white));
        connect(fillColorToolButton, &QAbstractButton::clicked,
                this, &MainWindow::fillButtonTriggered);

        lineColorToolButton = new QToolButton;
        lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
        lineAction = lineColorToolButton->menu()->defaultAction();
        lineColorToolButton->setIcon(createColorToolButtonIcon(
                ":/images/linecolor.png", Qt::black));
        connect(lineColorToolButton, &QAbstractButton::clicked,
                this, &MainWindow::lineButtonTriggered);

        textToolBar = addToolBar(tr("Font"));
        textToolBar->addWidget(fontCombo);
        textToolBar->addWidget(fontSizeCombo);
        textToolBar->addAction(boldAction);
        textToolBar->addAction(italicAction);
        textToolBar->addAction(underlineAction);

        colorToolBar = addToolBar(tr("Color"));
        colorToolBar->addWidget(fontColorToolButton);
        colorToolBar->addWidget(fillColorToolButton);
        colorToolBar->addWidget(lineColorToolButton);

        QToolButton* pointerButton = new QToolButton;
        pointerButton->setCheckable(true);
        pointerButton->setChecked(true);
        pointerButton->setIcon(QIcon(":/images/pointer.png"));
        QToolButton* linePointerButton = new QToolButton;
        linePointerButton->setCheckable(true);
        linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

        pointerTypeGroup = new QButtonGroup(this);
        pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
        pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
        connect(pointerTypeGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
                this, &MainWindow::pointerGroupClicked);

        sceneScaleCombo = new QComboBox;
        QStringList scales;
        scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
        sceneScaleCombo->addItems(scales);
        sceneScaleCombo->setCurrentIndex(2);
        connect(sceneScaleCombo, &QComboBox::currentTextChanged,
                this, &MainWindow::sceneScaleChanged);

        pointerToolbar = addToolBar(tr("Pointer type"));
        pointerToolbar->addWidget(pointerButton);
        pointerToolbar->addWidget(linePointerButton);
        pointerToolbar->addWidget(sceneScaleCombo);
    }

    QWidget* MainWindow::createBackgroundCellWidget(const QString& text, const QString& image) {
        QToolButton* button = new QToolButton;
        button->setText(text);
        button->setIcon(QIcon(image));
        button->setIconSize(QSize(50, 50));
        button->setCheckable(true);
        backgroundButtonGroup->addButton(button);

        QGridLayout* layout = new QGridLayout;
        layout->addWidget(button, 0, 0, Qt::AlignHCenter);
        layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

        QWidget* widget = new QWidget;
        widget->setLayout(layout);

        return widget;
    }

    QWidget* MainWindow::createCellWidget(const QString& text, DiagramItem::DiagramType type) {
        DiagramItem item(type, itemMenu);
        QIcon icon(item.image());

        QToolButton* button = new QToolButton;
        button->setIcon(icon);
        button->setIconSize(QSize(50, 50));
        button->setCheckable(true);
        buttonGroup->addButton(button, int(type));

        QGridLayout* layout = new QGridLayout;
        layout->addWidget(button, 0, 0, Qt::AlignHCenter);
        layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

        QWidget* widget = new QWidget;
        widget->setLayout(layout);

        return widget;
    }

    QMenu* MainWindow::createColorMenu(const char* slot, QColor defaultColor) {
        QList<QColor> colors;
        colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
        QStringList names;
        names << tr("black") << tr("white") << tr("red") << tr("blue")
              << tr("yellow");

        QMenu* colorMenu = new QMenu(this);
        for (int i = 0; i < colors.count(); ++i) {
            QAction* action = new QAction(names.at(i), this);
            action->setData(colors.at(i));
            action->setIcon(createColorIcon(colors.at(i)));
            connect(action, SIGNAL(triggered()), this, slot);
            colorMenu->addAction(action);
            if (colors.at(i) == defaultColor)
                colorMenu->setDefaultAction(action);
        }
        return colorMenu;
    }

    QIcon MainWindow::createColorToolButtonIcon(const QString& imageFile, QColor color) {
        QPixmap pixmap(50, 80);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        QPixmap image(imageFile);
        // Draw icon centred horizontally on button.
        QRect target(4, 0, 42, 43);
        QRect source(0, 0, 42, 43);
        painter.fillRect(QRect(0, 60, 50, 80), color);
        painter.drawPixmap(target, image, source);

        return QIcon(pixmap);
    }

    QIcon MainWindow::createColorIcon(QColor color) {
        QPixmap pixmap(20, 20);
        QPainter painter(&pixmap);
        painter.setPen(Qt::NoPen);
        painter.fillRect(QRect(0, 0, 20, 20), color);

        return QIcon(pixmap);
    }
}
