//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_ACTION_GROUP_MANAGER_H
#define NEWCUT_ACTION_GROUP_MANAGER_H

#include <QObject>

#include "newcut/application_window.h"

namespace nc {
    class ActionGroupManager : public QObject {
        Q_OBJECT
    public:
        explicit ActionGroupManager(ApplicationWindow* parent);
    };
}


#endif //NEWCUT_ACTION_GROUP_MANAGER_H
