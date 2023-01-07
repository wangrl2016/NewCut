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

        // File

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

        sub_menu = file_menu_->addMenu(ApplicationWindow::tr("Import"));
        sub_menu->setObjectName("Import");
        sub_menu->addAction(action_map_["DrawImage"]);
        sub_menu->addAction(action_map_["BlocksImport"]);
        sub_menu = file_menu_->addMenu(ApplicationWindow::tr("Export"));
        sub_menu->setObjectName("Export");
        sub_menu->addAction(action_map_["FileExportMakerCam"]);
        sub_menu->addAction(action_map_["FilePrintPDF"]);
        sub_menu->addAction(action_map_["FileExport"]);
        file_menu_->addSeparator();
        file_menu_->addAction(action_map_["FilePrint"]);
        file_menu_->addAction(action_map_["FilePrintPreview"]);
        file_menu_->addSeparator();
        file_menu_->addAction(action_map_["FileCloseAll"]);
        file_menu_->addAction(action_map_["FileQuit"]);
        file_menu_->addSeparator();

        // Options
        auto* settings_menu = new QMenu(ApplicationWindow::tr("&Options"), menu_bar);
        settings_menu->setObjectName("OptionsMenu");
        settings_menu->setTearOffEnabled(true);
        settings_menu->addAction(action_map_["OptionsGeneral"]);
        settings_menu->addAction(action_map_["OptionsDrawing"]);
        settings_menu->addAction(action_map_["WidgetOptions"]);
        settings_menu->addAction(action_map_["DeviceOptions"]);
        settings_menu->addAction(action_map_["ReloadStyleSheet"]);

        // Edit

        auto edit_menu = new QMenu(ApplicationWindow::tr("&Edit"), menu_bar);
        edit_menu->setObjectName("Edit");
        edit_menu->setTearOffEnabled(true);
        edit_menu->addAction(action_map_["EditKillAllActions"]);
        edit_menu->addSeparator();
        edit_menu->addAction(action_map_["EditUndo"]);
        edit_menu->addAction(action_map_["EditRedo"]);
        edit_menu->addSeparator();
        edit_menu->addAction(action_map_["EditCut"]);
        edit_menu->addAction(action_map_["EditCopy"]);
        edit_menu->addAction(action_map_["EditPaste"]);
        edit_menu->addAction(action_map_["ModifyDeleteQuick"]);

        auto plugins_menu = new QMenu(ApplicationWindow::tr("Plu&gins"), menu_bar);
        plugins_menu->setObjectName("PluginsMenu");
        plugins_menu->setTearOffEnabled(true);

        // View

        auto view_menu = new QMenu(ApplicationWindow::tr("&View"), menu_bar);
        view_menu->setObjectName("ViewMenu");
        view_menu->setTearOffEnabled(true);
        view_menu->addAction(action_map_["Fullscreen"]);
        view_menu->addAction(action_map_["viewStatusBar"]);
        view_menu->addAction(action_map_["ViewGrid"]);
        view_menu->addAction(action_map_["ViewDraft"]);
        view_menu->addSeparator();
        view_menu->addAction(action_map_["ZoomRedraw"]);
        view_menu->addAction(action_map_["ZoomIn"]);
        view_menu->addAction(action_map_["ZoomOut"]);
        view_menu->addAction(action_map_["ZoomAuto"]);
        view_menu->addAction(action_map_["ZoomPrevious"]);
        view_menu->addAction(action_map_["ZoomWindow"]);
        view_menu->addAction(action_map_["ZoomPan"]);

        // Tools
        auto tools_menu = new QMenu(ApplicationWindow::tr("&Tools"), menu_bar);
        tools_menu->setObjectName("ToolsMenu");
        tools_menu->setTearOffEnabled(true);

        // Lines
        sub_menu = tools_menu->addMenu(ApplicationWindow::tr("&Line"));
        sub_menu->setObjectName("Line");
        // sub_menu->addAction()

        auto help_menu = new QMenu(ApplicationWindow::tr("&Help"), menu_bar);
        help_menu->setObjectName("Help");
        help_menu->setTearOffEnabled(true);

        QAction* help_about = new QAction(ApplicationWindow::tr("About"), application_window_);
        connect(help_about, SIGNAL(triggered()),
                application_window_, SLOT(ShowAboutWindow()));
        help_menu->addAction(help_about);


        menu_bar->addMenu(file_menu_);
        menu_bar->addMenu(settings_menu);
        menu_bar->addMenu(edit_menu);
        menu_bar->addMenu(view_menu);
        menu_bar->addMenu(plugins_menu);
        menu_bar->addMenu(tools_menu);
        menu_bar->addMenu(help_menu);
    }
}