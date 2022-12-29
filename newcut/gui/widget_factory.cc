//
// Created by wr on 2022/12/28.
//

#include "newcut/gui/widget_factory.h"
#include "newcut/application_window.h"

namespace nc {
    WidgetFactory::WidgetFactory(nc::ApplicationWindow* window,
                                 QMap<QString, QAction*>& action_map,
                                 nc::ActionGroupManager* agm)
            : QObject(nullptr),
              application_window_(window),
              action_map_(action_map),
              action_group_manager_(agm) {
        file_actions_ << action_map["FileNew"]
                      << action_map["FileNewTemplate"]
                      << action_map["FileOpen"]
                      << action_map["FileSave"]
                      << action_map["FileSaveAs"]
                      << action_map["FileSaveAll"];
    }

    void WidgetFactory::CreateMenus(QMenuBar* menu_bar) {
        QMenu* sub_menu;

        file_menu_ = new QMenu(ApplicationWindow::tr("&File"), menu_bar);
        file_menu_->setObjectName("File");
        file_menu_->setTearOffEnabled(true);
        file_menu_->addAction(action_map_["FileNew"]);
        file_menu_->addAction(action_map_["FileNewTemplate"]);
        file_menu_->addAction(action_map_["FileOpen"]);
        file_menu_->addSeparator();
        file_menu_->addAction(action_map_["FileSave"]);
        file_menu_->addAction(action_map_["FileSaveAs"]);
        file_menu_->addAction(action_map_["FileSaveAll"]);
        file_menu_->addSeparator();

        menu_bar->addMenu(file_menu_);
    }
}