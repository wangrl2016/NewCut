//
// Created by wangrl2016 on 2022/11/30.
//

#ifndef NEWCUT_MAIN_WINDOW_H
#define NEWCUT_MAIN_WINDOW_H

#include <QButtonGroup>
#include <QMainWindow>
#include <QGraphicsView>
#include <QToolBar>
#include <QAbstractButton>
#include <QComboBox>
#include <QFontComboBox>
#include <QToolBox>
#include <QToolButton>
#include "diagram_item.h"
#include "diagram_scene.h"

namespace nc {
    class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow();

        ~MainWindow() override;

    private slots:

        void backgroundButtonGroupClicked(QAbstractButton* button);

        void buttonGroupClicked(QAbstractButton* button);

        void deleteItem();

        void pointerGroupClicked();

        void bringToFront();

        void sendToBack();

        void itemInserted(nc::DiagramItem* item);

        void textInserted(QGraphicsTextItem* item);

        void currentFontChanged(const QFont& font);

        void fontSizeChanged(const QString& size);

        void sceneScaleChanged(const QString& scale);

        void textColorChanged();

        void itemColorChanged();

        void lineColorChanged();

        void textButtonTriggered();

        void fillButtonTriggered();

        void lineButtonTriggered();

        void handleFontChange();

        void itemSelected(QGraphicsItem* item);

        void about();

    private:
        void createToolBox();

        void createActions();

        void createMenus();

        void createToolBars();

        QWidget* createBackgroundCellWidget(const QString& text,
                                            const QString& image);

        QWidget* createCellWidget(const QString& text,
                                  DiagramItem::DiagramType type);

        QMenu* createColorMenu(const char* slot,
                               QColor defaultColor);

        QIcon createColorToolButtonIcon(const QString& image,
                                        QColor color);

        QIcon createColorIcon(QColor color);

        DiagramScene* scene;
        QGraphicsView* view;

        // ??????
        QAction* exitAction;
        QAction* addAction;
        QAction* deleteAction;

        QAction* toFrontAction;
        QAction* sendBackAction;
        QAction* aboutAction;

        // ?????????
        QMenu* fileMenu;
        QMenu* itemMenu;
        QMenu* aboutMenu;

        // ?????????
        QToolBar* textToolBar;
        QToolBar* editToolBar;
        QToolBar* colorToolBar;
        QToolBar* pointerToolbar;

        QComboBox* sceneScaleCombo;
        QComboBox* itemColorCombo;
        QComboBox* textColorCombo;
        QComboBox* fontSizeCombo;
        QFontComboBox* fontCombo;

        QToolBox* toolBox;
        QButtonGroup* buttonGroup;
        QButtonGroup* pointerTypeGroup;
        QButtonGroup* backgroundButtonGroup;
        QToolButton* fontColorToolButton;
        QToolButton* fillColorToolButton;
        QToolButton* lineColorToolButton;

        QAction* boldAction;
        QAction* underlineAction;
        QAction* italicAction;
        QAction* textAction;
        QAction* fillAction;
        QAction* lineAction;
    };
}

#endif //NEWCUT_MAIN_WINDOW_H
