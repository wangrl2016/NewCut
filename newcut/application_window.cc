//
// Created by wangrl2016 on 2022/12/28.
//

#include <glog/logging.h>
#include <QDialog>
#include <QSettings>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QApplication>
#include <QLabel>
#include "build_config.h"
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

    void ApplicationWindow::ShowAboutWindow() {
        QDialog dialog;
        dialog.setWindowTitle(tr("About"));

        auto layout = new QVBoxLayout;
        dialog.setLayout(layout);

        auto frame = new QGroupBox(qApp->applicationName());
        layout->addWidget(frame);

        auto f_layout = new QVBoxLayout;
        frame->setLayout(f_layout);

        QString info(
                tr("Version: %1.%2.%3").arg(NewCut_VERSION_MAJOR)
                        .arg(NewCut_VERSION_MINOR)
                        .arg(NewCut_VERSION_PATCH) + "\n" +
                #if defined(Q_CC_CLANG)
                tr("Compiler: Clang %1.%2.%3").arg(__clang_major__)
                        .arg(__clang_minor__)
                        .arg(__clang_patchlevel__) + "\n" +
                #elif defined(Q_CC_GNU)
                tr("Compiler: GNU GCC %1.%2.%3").arg(__GNUC__)
                        .arg(__GNUC_MIN_OR)
                        .arg(__GNUC_PATCHLEVEL__) + "\n" +
                #elif defined(Q_CC_MSVC)
                tr("Compiler: Microsoft Visual C++") + "\n" +
                #endif
                tr("Compiled on: %1").arg(__DATE__) + "\n" +
                tr("Qt Version: %1").arg(qVersion())
        );
        auto app_info = new QLabel(info);
        app_info->setTextInteractionFlags(Qt::TextSelectableByMouse);
        f_layout->addWidget(app_info);

        dialog.exec();
    }
}
