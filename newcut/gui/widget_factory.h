//
// Created by wr on 2022/12/28.
//

#ifndef NEWCUT_WIDGET_FACTORY_H
#define NEWCUT_WIDGET_FACTORY_H

#include <QObject>
#include <QAction>
#include <QMenuBar>

namespace nc {
    class ActionGroupManager;

    class ApplicationWindow;

    // 创建所有的Widget，包括MenuBar
    class WidgetFactory : public QObject {
    Q_OBJECT
    public:
        WidgetFactory(ApplicationWindow* window,
                      QMap<QString, QAction*>& action_map,
                      ActionGroupManager* agm);

        void CreateMenus(QMenuBar* menu_bar);

        QMenu* file_menu_;

    private:
        ApplicationWindow* application_window_;
        QMap<QString, QAction*>& action_map_;
        ActionGroupManager* action_group_manager_;

        QList<QAction*> file_actions_;

    };
}


#endif //NEWCUT_WIDGET_FACTORY_H
