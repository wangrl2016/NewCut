//
// Created by wangrl2016 on 2022/12/28.
//

#include <glog/logging.h>
#include "newcut/application_window.h"
#include "newcut/gui/widget_factory.h"

namespace nc {
    ApplicationWindow* ApplicationWindow::app_window_ = nullptr;

    ApplicationWindow::ApplicationWindow() {
        LOG(INFO) << __FUNCTION__;

        // accept drop events to open files
        setAcceptDrops(true);

        // make the left and right dock areas dominant
        setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

        app_window_ = this;

        LOG(INFO) << __FUNCTION__ << " setting icon";

        // QStatusBar* status_bar = statusBar();

        WidgetFactory widget_factory(this, action_map_, action_group_manager_);

        widget_factory.CreateMenus(menuBar());
    }

    ApplicationWindow::~ApplicationWindow() {

    }
}