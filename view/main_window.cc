//
// Created by wr on 2022/12/21.
//

#include <glog/logging.h>
#include <QHBoxLayout>
#include "view/main_window.h"

namespace nc {
    constexpr int kSceneLength = 5000.0f;

    MainWindow::MainWindow() {
        LOG(INFO) << __FUNCTION__;
        // 轨道窗
        track_graphics_scene_ = new TrackGraphicsScene(this);
        track_graphics_scene_->setSceneRect(QRectF(0, 0, kSceneLength, kSceneLength));
        track_graphics_view_ = new TrackGraphicsView(track_graphics_scene_, this);

        // 钢琴窗
        piano_graphics_scene_ = new PianoGraphicsScene(this);
        piano_graphics_scene_->setSceneRect(QRectF(0, 0, kSceneLength, kSceneLength));
        piano_graphics_view_ = new PianoGraphicsView(piano_graphics_scene_, this);

        // 参数窗
        param_widget_ = new ParamWidget(this);
        param_widget_->setMinimumHeight(height() / 4);

        // 从上到下的布局
        auto* layout = new QVBoxLayout();
        layout->addWidget(track_graphics_view_);
        layout->addWidget(piano_graphics_view_);
        layout->addWidget(param_widget_);

        auto* widget = new QWidget();
        widget->setLayout(layout);

        setCentralWidget(widget);
        setUnifiedTitleAndToolBarOnMac(true);
    }

    MainWindow::~MainWindow() {
        LOG(INFO) << __FUNCTION__;
    }
}
