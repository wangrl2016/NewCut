//
// Created by wangrl2016 on 2022/11/30.
//

#ifndef NEWCUT_MAIN_WINDOW_H
#define NEWCUT_MAIN_WINDOW_H

#include <QMainWindow>

namespace nc {
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow();

        ~MainWindow();

    private slots:

    private:
        void createToolBox();

        void createActions();

        void createMenus();

        void createToolBars();

        QAction* exitAction;

        QMenu* fileMenu;
        QMenu* itemMenu;
        QMenu* aboutMenu;
    };
}

#endif //NEWCUT_MAIN_WINDOW_H
