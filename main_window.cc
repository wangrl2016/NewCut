#include <QtWidgets>
#include "main_window.h"

namespace nc {
    MainWindow::MainWindow() :
            text_edit_(new QPlainTextEdit) {
        setCentralWidget(text_edit_);
        CreateActions();
        CreateStatusBar();

        ReadSettings();

        connect(text_edit_->document(), &QTextDocument::contentsChanged,
                this, &MainWindow::DocumentWasModified);

        SetCurrentFile(QString());
        setUnifiedTitleAndToolBarOnMac(true);
    }

    void MainWindow::LoadFile(const QString& filename) {
        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                         .arg(QDir::toNativeSeparators(filename),
                                              file.errorString()));
            return;
        }

        QTextStream in(&file);
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        text_edit_->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif

        SetCurrentFile(filename);
        statusBar()->showMessage(tr("File loaded"), 2000);
    }

    void MainWindow::closeEvent(QCloseEvent* event) {
        if (MaybeSave()) {
            WriteSettings();
            event->accept();
        } else {
            event->ignore();
        }
    }

    void MainWindow::NewFile() {
        if (MaybeSave()) {
            text_edit_->clear();
            SetCurrentFile(QString());
        }
    }

    void MainWindow::Open() {
        if (MaybeSave()) {
            QString fileName = QFileDialog::getOpenFileName(this);
            if (!fileName.isEmpty())
                LoadFile(fileName);
        }
    }

    bool MainWindow::Save() {
        if (cur_file_.isEmpty()) {
            return SaveAs();
        } else {
            return SaveFile(cur_file_);
        }
    }

    bool MainWindow::SaveAs() {
        QFileDialog dialog(this);
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        if (dialog.exec() != QDialog::Accepted)
            return false;
        return SaveFile(dialog.selectedFiles().first());
    }

    void MainWindow::About() {
        QMessageBox::about(this, tr("About Application"),
                           tr("The <b>Application</b> example demonstrates how to "
                              "write modern GUI applications using Qt, with a menu bar, "
                              "toolbars, and a status bar."));
    }

    void MainWindow::DocumentWasModified() {
        setWindowModified(text_edit_->document()->isModified());
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
        if (!text_edit_->document()->isModified())
            return true;
        const QMessageBox::StandardButton ret
                = QMessageBox::warning(this, tr("Application"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
            case QMessageBox::Save:
                return Save();
            case QMessageBox::Cancel:
                return false;
            default:
                break;
        }
        return true;
    }

    bool MainWindow::SaveFile(const QString& filename) {
        QString errorMessage;

        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
        QSaveFile file(filename);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out << text_edit_->toPlainText();
            if (!file.commit()) {
                errorMessage = tr("Cannot write file %1:\n%2.")
                        .arg(QDir::toNativeSeparators(filename), file.errorString());
            }
        } else {
            errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                    .arg(QDir::toNativeSeparators(filename), file.errorString());
        }
        QGuiApplication::restoreOverrideCursor();

        if (!errorMessage.isEmpty()) {
            QMessageBox::warning(this, tr("Application"), errorMessage);
            return false;
        }

        SetCurrentFile(filename);
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    }

    void MainWindow::SetCurrentFile(const QString& filename) {
        cur_file_ = filename;
        text_edit_->document()->setModified(false);
        setWindowModified(false);

        QString shownName = cur_file_;
        if (cur_file_.isEmpty())
            shownName = "untitled.txt";
        setWindowFilePath(shownName);
    }

    QString MainWindow::StrippedName(const QString& full_filename) {
        return QFileInfo(full_filename).fileName();
    }
}

