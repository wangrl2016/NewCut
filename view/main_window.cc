//
// Created by wr on 2022/12/21.
//

#include <QHBoxLayout>
#include "view/main_window.h"

namespace nc {
    constexpr int kSceneLength = 5000.0f;

    MainWindow::MainWindow() {
        // 构建绘制区域
        params_graphics_scene_ = new ParamsGraphicsScene(this);
        params_graphics_scene_->setSceneRect(
                QRectF(0, 0, kSceneLength, kSceneLength));

        params_graphics_view_ = new ParamsGraphicsView(params_graphics_scene_, this);

        auto* layout = new QHBoxLayout();
        layout->addWidget(params_graphics_view_);

        auto* widget = new QWidget();
        widget->setLayout(layout);

        setCentralWidget(widget);
        setUnifiedTitleAndToolBarOnMac(true);
    }

    MainWindow::~MainWindow() {

    }
}