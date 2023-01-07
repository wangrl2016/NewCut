//
// Created by wangrl2016 on 2022/12/28.
//

#include "newcut/gui/action_group_manager.h"

namespace nc {
    ActionGroupManager::ActionGroupManager(nc::ApplicationWindow* parent)
            : QObject(parent),
              block(new QActionGroup(this)),
              circle(new QActionGroup(this)),
              curve(new QActionGroup(this)),
              edit(new QActionGroup(this)),
              ellipse(new QActionGroup(this)),
              file(new QActionGroup(this)),
              dimension(new QActionGroup(this)),
              info(new QActionGroup(this)),
              layer(new QActionGroup(this)),
              line(new QActionGroup(this)),
              modify(new QActionGroup(this)),
              options(new QActionGroup(this)),
              other(new QActionGroup(this)),
              polyline(new QActionGroup(this)),
              restriction(new QActionGroup(this)),
              select(new QActionGroup(this)),
              snap(new QActionGroup(this)),
              snap_extras(new QActionGroup(this)),
              view(new QActionGroup(this)),
              widgets(new QActionGroup(this)) {
        block->setObjectName(QObject::tr("Block"));
        circle->setObjectName(QObject::tr("Circle"));
        curve->setObjectName(QObject::tr("Curve"));
        edit->setObjectName(QObject::tr("Edit"));
        ellipse->setObjectName(QObject::tr("Ellipse"));
        file->setObjectName(QObject::tr("File"));
        dimension->setObjectName(QObject::tr("Dimension"));
        info->setObjectName(QObject::tr("Info"));
        layer->setObjectName(QObject::tr("Layer"));
        line->setObjectName(QObject::tr("Line"));
        modify->setObjectName(QObject::tr("Modify"));
        options->setObjectName(QObject::tr("Options"));
        other->setObjectName(QObject::tr("Other"));
        polyline->setObjectName(QObject::tr("Polyline"));
        restriction->setObjectName(QObject::tr("Restriction"));
        select->setObjectName(QObject::tr("Select"));
        snap->setObjectName(QObject::tr("Snap"));
        snap_extras->setObjectName(QObject::tr("Snap Extras"));
        view->setObjectName(QObject::tr("View"));
        widgets->setObjectName(QObject::tr("Widgets"));
    }
}
