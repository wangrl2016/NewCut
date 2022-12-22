//
// Created by wangrl2016 on 2022/12/22.
//

#include <glog/logging.h>
#include "view/piano/piano_graphics_scene.h"

namespace nc {
    PianoGraphicsScene::PianoGraphicsScene(QObject* parent)
            : QGraphicsScene(parent) {
        LOG(INFO) << __FUNCTION__;
    }

    PianoGraphicsScene::~PianoGraphicsScene() {
        LOG(INFO) << __FUNCTION__;
    }

    void PianoGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        LOG(INFO) << __FUNCTION__;
    }

    void PianoGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
        LOG(INFO) << __FUNCTION__;
    }

    void PianoGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        LOG(INFO) << __FUNCTION__;
    }
}
