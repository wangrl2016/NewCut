//
// Created by wr on 2022/12/28.
//

#ifndef NEWCUT_MAIN_WINDOW_H
#define NEWCUT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>

namespace nc {
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget* parent = nullptr);

        void SortWidgetsByTitle(QList<QDockWidget*>& list);

        void SortWidgetsByTitle(QList<QToolBar*>& list);

    public slots:
        void ToggleRightDockArea(bool state);

        void ToggleLeftDockArea(bool state);

        void ToggleTopDockArea(bool state);

        void ToggleBottomDockArea(bool state);

        void ToggleFloatingDockWidgets(bool state);
    };
}


#endif //NEWCUT_MAIN_WINDOW_H
