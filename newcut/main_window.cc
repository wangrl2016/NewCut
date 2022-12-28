//
// Created by wr on 2022/12/28.
//

#include <algorithm>
#include "newcut/main_window.h"

namespace nc {
    bool ByWindowTitle(QWidget* left, QWidget* right) {
        return left->windowTitle() < right->windowTitle();
    }

    MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {}

    void MainWindow::SortWidgetsByTitle(QList<QDockWidget*>& list) {
        std::sort(list.begin(), list.end(), ByWindowTitle);
    }

    void MainWindow::SortWidgetsByTitle(QList<QToolBar*>& list) {
        std::sort(list.begin(), list.end(), ByWindowTitle);
    }

    void MainWindow::ToggleLeftDockArea(bool state) {

    }

    void MainWindow::ToggleRightDockArea(bool state) {

    }

    void MainWindow::ToggleTopDockArea(bool state) {

    }

    void MainWindow::ToggleBottomDockArea(bool state) {

    }

    void MainWindow::ToggleFloatingDockWidgets(bool state) {

    }
}
