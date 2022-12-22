//
// Created by wangrl2016 on 2022/12/22.
//

#include <glog/logging.h>
#include "view/tracks/track_graphics_scene.h"

namespace nc {
    TrackGraphicsScene::TrackGraphicsScene(QObject* parent)
            : QGraphicsScene(parent) {
        LOG(INFO) << __FUNCTION__;
    }

    TrackGraphicsScene::~TrackGraphicsScene() {
        LOG(INFO) << __FUNCTION__;
    }
}