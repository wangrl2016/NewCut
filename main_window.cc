#include <QtWidgets>
#include "main_window.h"

namespace nc {
    MainWindow::MainWindow() :
            text_edit_(new QPlainTextEdit) {
        setCentralWidget(text_edit_);
        CreateActions();
        CreateStatusBar();

        ReadSettings();
    }

    void MainWindow::LoadFile(const QString& filename) {

    }

    void MainWindow::closeEvent(QCloseEvent* event) {

    }

    void MainWindow::NewFile() {

    }

    void MainWindow::Open() {

    }

    void MainWindow::Save() {

    }

    void MainWindow::SaveAs() {

    }

    void MainWindow::About() {

    }

    void MainWindow::DocumentWasModified() {

    }

    void MainWindow::CreateActions() {
        // new
        QMenu* file_menu = menuBar()->addMenu(tr("&File"));
        QToolBar* file_tool_bar = addToolBar(tr("File"));
        const QIcon new_icon = QIcon::fromTheme("document-new",
                                                QIcon(":images/new.png"));
        QAction* new_act = new QAction(new_icon, tr("&New"), this);
        new_act->setShortcuts(QKeySequence::New);
        new_act->setStatusTip(tr("Create a new file"));
        connect(new_act, &QAction::triggered, this, &MainWindow::NewFile);
        file_menu->addAction(new_act);
        file_tool_bar->addAction(new_act);

        // open
        const QIcon open_icon = QIcon::fromTheme("docuemnt-open",
                                                 QIcon(":/images/open.png"));
        QAction* open_act = new QAction(open_icon, tr("&Open..."), this);
        open_act->setShortcuts(QKeySequence::Open);
        open_act->setStatusTip(tr("Open an existing file"));
        connect(open_act, &QAction::triggered, this, &MainWindow::Open);
        file_menu->addAction(open_act);
        file_tool_bar->addAction(open_act);

        // save
        const QIcon saveIcon = QIcon::fromTheme("document-save",
                                                QIcon(":/images/save.png"));
        QAction* save_act = new QAction(saveIcon, tr("&Save"), this);
        save_act->setShortcuts(QKeySequence::Save);
        save_act->setStatusTip(tr("Save the document to disk"));
        connect(save_act, &QAction::triggered, this, &MainWindow::Save);
        file_menu->addAction(save_act);
        file_tool_bar->addAction(save_act);

        // save as
        const QIcon save_as_icon = QIcon::fromTheme("document-save-as");
        QAction* save_as_act = file_menu->addAction(save_as_icon,
                                                    tr("Save &As..."), this, &MainWindow::SaveAs);
        save_as_act->setShortcuts(QKeySequence::SaveAs);
        save_as_act->setStatusTip(tr("Save the document under a new name"));

        file_menu->addSeparator();

        const QIcon exit_icon = QIcon::fromTheme("application-exit");
        QAction* exit_act = file_menu->addAction(exit_icon,
                                                 tr("E&xit"), this, &QWidget::close);
        exit_act->setShortcuts(QKeySequence::Quit);
        exit_act->setStatusTip(tr("Exit the application"));

        QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
        QToolBar *editToolBar = addToolBar(tr("Edit"));

        const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
        QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
        cutAct->setShortcuts(QKeySequence::Cut);
        cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                                "clipboard"));
        connect(cutAct, &QAction::triggered, text_edit_, &QPlainTextEdit::cut);
        editMenu->addAction(cutAct);
        editToolBar->addAction(cutAct);

        const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
        QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
        copyAct->setShortcuts(QKeySequence::Copy);
        copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                                 "clipboard"));
        connect(copyAct, &QAction::triggered, text_edit_, &QPlainTextEdit::copy);
        editMenu->addAction(copyAct);
        editToolBar->addAction(copyAct);

        const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
        QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
        pasteAct->setShortcuts(QKeySequence::Paste);
        pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                                  "selection"));
        connect(pasteAct, &QAction::triggered, text_edit_, &QPlainTextEdit::paste);
        editMenu->addAction(pasteAct);
        editToolBar->addAction(pasteAct);

        menuBar()->addSeparator();

        QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
        QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::About);
        aboutAct->setStatusTip(tr("Show the application's About box"));

        QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
        aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

        cutAct->setEnabled(false);
        copyAct->setEnabled(false);
        connect(text_edit_, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
        connect(text_edit_, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
    }

    void MainWindow::CreateStatusBar() {
        statusBar()->showMessage(tr("Ready"));
    }

    void MainWindow::ReadSettings() {
        QSettings settings(QCoreApplication::organizationName(),
                           QCoreApplication::applicationName());
        const QByteArray geometry = settings.value("geometry",
                                                   QByteArray()).toByteArray();
        if (geometry.isEmpty()) {
            const QRect availableGeometry = screen()->availableGeometry();
            resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
            move((availableGeometry.width() - width()) / 2,
                 (availableGeometry.height() - height()) / 2);
        } else {
            restoreGeometry(geometry);
        }
    }

    void MainWindow::WriteSettings() {
        QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        settings.setValue("geometry", saveGeometry());
    }

    bool MainWindow::MaybeSave() {
        return false;
    }

    bool MainWindow::SaveFile(const QString& filename) {
        return false;
    }

    void MainWindow::SetCurrentFile(const QString& filename) {

    }

    QString MainWindow::StrippedName(const QString& full_filename) {
        return QString();
    }
}

