//
// Created by wangrl2016 on 2022/12/22.
//

#include <glog/logging.h>
#include "view/params/params_graphics_view.h"

namespace nc {
    ParamsGraphicsView::ParamsGraphicsView(QGraphicsScene* scene,
                                           QWidget* parent)
            : QGraphicsView(scene, parent) {

    }

    ParamsGraphicsView::~ParamsGraphicsView() {
        LOG(INFO) << __FUNCTION__;
    }
}