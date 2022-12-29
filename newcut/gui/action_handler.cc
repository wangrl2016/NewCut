//
// Created by wangrl2016 on 2022/12/29.
//

#include <glog/logging.h>
#include "newcut/gui/action_handler.h"

namespace nc {
    ActionHandler::ActionHandler(QObject* parent)
            : QObject(parent) {
        LOG(INFO) << __FUNCTION__ << " OK";
    }


}
