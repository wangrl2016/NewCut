//
// Created by wangrl2016 on 2022/12/29.
//

#ifndef NEWCUT_ACTION_FACTORY_H
#define NEWCUT_ACTION_FACTORY_H

#include <QObject>
#include <QAction>

namespace nc {
    class ApplicationWindow;
    class ActionGroupManager;
    class ActionHandler;

    // 创建所有的Action
    class ActionFactory : public QObject {
        Q_OBJECT
    public:
        ActionFactory(ApplicationWindow* parent, ActionHandler* handler);

        void FillActionContainer(QMap<QString, QAction*>& action_map,
                                 ActionGroupManager* agm);

        void CommonActions(QMap<QString, QAction*>& action_map,
                           ActionGroupManager* agm);

        bool using_theme_;

    private:
        ApplicationWindow* application_window_;
        ActionHandler* action_handler_;
    };
}

#endif //NEWCUT_ACTION_FACTORY_H
