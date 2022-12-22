//
// Created by wr on 2022/12/21.
//

#ifndef NEWCUT_MAIN_WINDOW_H
#define NEWCUT_MAIN_WINDOW_H

#include <QMainWindow>
#include "view/params/params_graphics_view.h"
#include "view/params/params_graphics_scene.h"

namespace nc {
    class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow();

        ~MainWindow() override;

    protected:

    private:
        ParamsGraphicsView* params_graphics_view_;
        ParamsGraphicsScene* params_graphics_scene_;
    };
}

#endif //NEWCUT_MAIN_WINDOW_H
