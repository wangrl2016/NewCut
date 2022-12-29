//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_ACTION_GROUP_MANAGER_H
#define NEWCUT_ACTION_GROUP_MANAGER_H

#include <QActionGroup>
#include <QObject>

#include "newcut/application_window.h"

namespace nc {
    // 将Action进行分类管理，比如文件相关的打开、关闭、新建
    class ActionGroupManager : public QObject {
    Q_OBJECT
    public:
        explicit ActionGroupManager(ApplicationWindow* parent);

        QActionGroup* block_;
        QActionGroup* circle_;
        QActionGroup* curve_;
        QActionGroup* edit_;
        QActionGroup* ellipse_;
        QActionGroup* file_;
        QActionGroup* dimension_;
        QActionGroup* info_;
        QActionGroup* layer_;
        QActionGroup* line_;
        QActionGroup* modify_;
        QActionGroup* options_;
        QActionGroup* other_;
        QActionGroup* polyline_;
        QActionGroup* restriction_;
        QActionGroup* select_;
        QActionGroup* snap_;
        QActionGroup* snap_extras_;
        QActionGroup* view_;
        QActionGroup* widgets_;

    private:
        QList<bool> snap_memory_;
    };
}

#endif //NEWCUT_ACTION_GROUP_MANAGER_H
