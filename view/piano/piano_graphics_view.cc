//
// Created by wangrl2016 on 2022/12/22.
//

#include <glog/logging.h>
#include "view/piano/piano_graphics_view.h"

namespace nc {
    PianoGraphicsView::PianoGraphicsView(QGraphicsScene* scene, QWidget* parent)
            : QGraphicsView(scene, parent) {
        LOG(INFO) << __FUNCTION__;
    }

    PianoGraphicsView::~PianoGraphicsView() {
        LOG(INFO) << __FUNCTION__;
    }
}