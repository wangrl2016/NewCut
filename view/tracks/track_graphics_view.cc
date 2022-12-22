//
// Created by wangrl2016 on 2022/12/22.
//

#include <glog/logging.h>
#include "view/tracks/track_graphics_view.h"

namespace nc {
    TrackGraphicsView::TrackGraphicsView(QGraphicsScene* scene, QWidget* widget)
            : QGraphicsView(scene, widget) {
        LOG(INFO) << __FUNCTION__;
    }

    TrackGraphicsView::~TrackGraphicsView() {
        LOG(INFO) << __FUNCTION__;
    }
}
