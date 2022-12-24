//
// Created by wr on 2022/12/21.
//

#ifndef NEWCUT_MAIN_WINDOW_H
#define NEWCUT_MAIN_WINDOW_H

#include <QMainWindow>

#include "view/params/param_widget.h"
#include "view/path_stroke/path_stroke_widget.h"
#include "view/piano/piano_graphics_view.h"
#include "view/piano/piano_graphics_scene.h"
#include "view/tracks/track_graphics_view.h"
#include "view/tracks/track_graphics_scene.h"

namespace nc {
    class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow();

        ~MainWindow() override;

    protected:

    private:
        // 轨道窗
        TrackGraphicsView* track_graphics_view_;
        TrackGraphicsScene* track_graphics_scene_;

        // 钢琴窗
        PianoGraphicsView* piano_graphics_view_;
        PianoGraphicsScene* piano_graphics_scene_;

        // 参数窗
        ParamWidget* param_widget_;

        // 画线测试
        PathStrokeWidget* path_stroke_widget_;
    };
}

#endif //NEWCUT_MAIN_WINDOW_H
