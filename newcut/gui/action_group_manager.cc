//
// Created by wangrl2016 on 2022/12/28.
//

#include "newcut/gui/action_group_manager.h"

namespace nc {
    ActionGroupManager::ActionGroupManager(nc::ApplicationWindow* parent)
            : QObject(parent),
              block_(new QActionGroup(this)),
              circle_(new QActionGroup(this)),
              curve_(new QActionGroup(this)),
              edit_(new QActionGroup(this)),
              ellipse_(new QActionGroup(this)),
              file_(new QActionGroup(this)),
              dimension_(new QActionGroup(this)),
              info_(new QActionGroup(this)),
              layer_(new QActionGroup(this)),
              line_(new QActionGroup(this)),
              modify_(new QActionGroup(this)),
              options_(new QActionGroup(this)),
              other_(new QActionGroup(this)),
              polyline_(new QActionGroup(this)),
              restriction_(new QActionGroup(this)),
              select_(new QActionGroup(this)),
              snap_(new QActionGroup(this)),
              snap_extras_(new QActionGroup(this)),
              view_(new QActionGroup(this)),
              widgets_(new QActionGroup(this)) {
        block_->setObjectName(QObject::tr("Block"));
        circle_->setObjectName(QObject::tr("Circle"));
        curve_->setObjectName(QObject::tr("Curve"));
        edit_->setObjectName(QObject::tr("Edit"));
        ellipse_->setObjectName(QObject::tr("Ellipse"));
        file_->setObjectName(QObject::tr("File"));
        dimension_->setObjectName(QObject::tr("Dimension"));
        info_->setObjectName(QObject::tr("Info"));
        layer_->setObjectName(QObject::tr("Layer"));
        line_->setObjectName(QObject::tr("Line"));
        modify_->setObjectName(QObject::tr("Modify"));
        options_->setObjectName(QObject::tr("Options"));
        other_->setObjectName(QObject::tr("Other"));
        polyline_->setObjectName(QObject::tr("Polyline"));
        restriction_->setObjectName(QObject::tr("Restriction"));
        select_->setObjectName(QObject::tr("Select"));
        snap_->setObjectName(QObject::tr("Snap"));
        snap_extras_->setObjectName(QObject::tr("Snap Extras"));
        view_->setObjectName(QObject::tr("View"));
        widgets_->setObjectName(QObject::tr("Widgets"));
    }
}
