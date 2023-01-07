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

        QActionGroup* block;
        QActionGroup* circle;
        QActionGroup* curve;
        QActionGroup* edit;
        QActionGroup* ellipse;
        QActionGroup* file;
        QActionGroup* dimension;
        QActionGroup* info;
        QActionGroup* layer;
        QActionGroup* line;
        QActionGroup* modify;
        QActionGroup* options;
        QActionGroup* other;
        QActionGroup* polyline;
        QActionGroup* restriction;
        QActionGroup* select;
        QActionGroup* snap;
        QActionGroup* snap_extras;
        QActionGroup* view;
        QActionGroup* widgets;

    private:
        QList<bool> snap_memory_;
    };
}

#endif //NEWCUT_ACTION_GROUP_MANAGER_H
