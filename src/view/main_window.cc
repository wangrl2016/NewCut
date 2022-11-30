//
// Created by wangrl2016 on 2022/11/30.
//

#include <glog/logging.h>
#include <QMenuBar>
#include "src/view/main_window.h"

namespace nc {
    MainWindow::MainWindow() {
        createActions();
        createMenus();
    }

    MainWindow::~MainWindow() {
        LOG(INFO) << __FUNCTION__;
    }

    void MainWindow::createToolBox() {

    }

    void MainWindow::createActions() {
        exitAction = new QAction(tr("Abort"), this);
        exitAction->setShortcuts(QKeySequence::Quit);
        exitAction->setStatusTip(tr("Quit Scene diagram example"));
    }

    void MainWindow::createMenus() {
        fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(exitAction);
    }

    void MainWindow::createToolBars() {

    }
}
