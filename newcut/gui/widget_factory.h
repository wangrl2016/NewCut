//
// Created by wr on 2022/12/28.
//

#ifndef NEWCUT_WIDGET_FACTORY_H
#define NEWCUT_WIDGET_FACTORY_H

#include <QObject>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>

namespace nc {
    class ActionHandler;
    class ActionGroupManager;
    class ApplicationWindow;

    // 创建所有的Widget，包括MenuBar
    class WidgetFactory : public QObject {
    Q_OBJECT
    public:
        WidgetFactory(ApplicationWindow* window,
                      QMap<QString, QAction*>& action_map,
                      ActionGroupManager* agm);

        void CreateStandardToolbars(ActionHandler* action_handler);

        void CreateCADToolBar();

        void CreateMenus(QMenuBar* menu_bar);

        void CreateLeftSideBar(int columns, int icon_size);

        void CreateRightSidebar(ActionHandler* action_handler);

        QToolBar* CreateCategoriesToolBar();

        QMenu* file_menu_;      // 最上层的菜单栏
        QMenu* windows_menu_;

    private:
        ApplicationWindow* application_window_;
        QMap<QString, QAction*>& action_map_;
        ActionGroupManager* action_group_manager_;

        QList<QAction*> file_actions_;
        QList<QAction*> line_actions_;
        QList<QAction*> circle_actions_;
        QList<QAction*> curve_actions_;
        QList<QAction*> ellipse_actions_;
        QList<QAction*> polyline_actions_;
        QList<QAction*> select_actions_;
        QList<QAction*> dimension_actions_;
        QList<QAction*> modify_actions_;
        QList<QAction*> info_actions_;
        QList<QAction*> layer_actions_;
        QList<QAction*> block_actions_;
    };
}

#endif //NEWCUT_WIDGET_FACTORY_H
