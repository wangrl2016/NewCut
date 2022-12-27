//
// Created by wangrl2016 on 2022/12/22.
//

#include <glog/logging.h>
#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include "view/params/param_widget.h"

namespace nc {
    ParamWidget::ParamWidget(QWidget* parent)
            : QWidget(parent) {
        LOG(INFO) << __FUNCTION__;
        // timer_.start(100, Qt::PreciseTimer, this);
    }

    ParamWidget::~ParamWidget() {
        LOG(INFO) << __FUNCTION__;
    }

    void ParamWidget::paintEvent(QPaintEvent* event) {
        QWidget::paintEvent(event);
        // LOG(INFO) << __FUNCTION__;
        QPainter painter(this);
        painter.setBrush(QColor(255, 0, 0, 127));
        painter.drawRect(0, 0, width(), height());
    }

    void ParamWidget::mouseDoubleClickEvent(QMouseEvent* event) {
        QWidget::mouseDoubleClickEvent(event);
    }

    void ParamWidget::mouseMoveEvent(QMouseEvent* event) {
        QWidget::mouseMoveEvent(event);
    }

    void ParamWidget::mousePressEvent(QMouseEvent* event) {
        QWidget::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {

        }
    }

    void ParamWidget::mouseReleaseEvent(QMouseEvent* event) {
        QWidget::mouseReleaseEvent(event);
    }

    void ParamWidget::resizeEvent(QResizeEvent* event) {
        QWidget::resizeEvent(event);
    }

    void ParamWidget::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);
    }

    void ParamWidget::timerEvent(QTimerEvent* event) {
        // LOG(INFO) << __FUNCTION__;
    }
}
