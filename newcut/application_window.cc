//
// Created by wangrl2016 on 2022/12/28.
//

#include <glog/logging.h>
#include <QSettings>
#include "newcut/application_window.h"
#include "newcut/gui/action_handler.h"
#include "newcut/gui/action_group_manager.h"
#include "newcut/gui/widget_factory.h"
#include "newcut/gui/action_factory.h"

namespace nc {
    ApplicationWindow* ApplicationWindow::app_window_ = nullptr;

    ApplicationWindow::ApplicationWindow()
            : action_group_manager_(new ActionGroupManager(this)),
            auto_save_timer_(nullptr),
            action_handler_(new ActionHandler(this)) {
        LOG(INFO) << __FUNCTION__;

        // accept drop events to open files
        setAcceptDrops(true);

        // make the left and right dock areas dominant
        setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
        setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

        app_window_ = this;

        QSettings settings;

        LOG(INFO) << __FUNCTION__ << " setting icon";

        // QStatusBar* status_bar = statusBar();

        ActionFactory action_factory(this, action_handler_);
        action_factory.using_theme_ = settings.value("Widgets/AllowTheme", 0).toBool();
        action_factory.FillActionContainer(action_map_, action_group_manager_);

        WidgetFactory widget_factory(this, action_map_, action_group_manager_);

        widget_factory.CreateMenus(menuBar());
    }

    ApplicationWindow::~ApplicationWindow() {

    }
}