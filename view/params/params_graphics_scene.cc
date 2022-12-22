//
// Created by wangrl2016 on 2022/12/22.
//

#include <glog/logging.h>
#include "view/params/params_graphics_scene.h"

namespace nc {
    ParamsGraphicsScene::ParamsGraphicsScene(QObject* parent)
            : QGraphicsScene(parent) {

    }

    ParamsGraphicsScene::~ParamsGraphicsScene() {
        LOG(INFO) << __FUNCTION__;
    }
}