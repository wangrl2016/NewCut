//
// Created by wangrl2016 on 2022/12/22.
//

#include <glog/logging.h>
#include <QColor>
#include <QPainter>
#include "view/params/params_widget.h"

namespace nc {
    ParamsWidget::ParamsWidget(QWidget* parent)
            : QWidget(parent) {

    }

    ParamsWidget::~ParamsWidget() {
        LOG(INFO) << __FUNCTION__;
    }

    void ParamsWidget::paintEvent(QPaintEvent* event) {
        LOG(INFO) << __FUNCTION__;
        QPainter painter(this);
        painter.setBrush(QColor(255, 0, 0, 127));
        painter.drawRect(0, 0, width() / 2, height());

        painter.setBrush(QColor(0, 0, 255, 127));
        painter.drawRect(0, 0, width(), height() / 2);
    }

    void ParamsWidget::mouseDoubleClickEvent(QMouseEvent* event) {
        // QWidget::mouseDoubleClickEvent(event);
    }

    void ParamsWidget::mouseMoveEvent(QMouseEvent* event) {
        // QWidget::mouseMoveEvent(event);
    }

    void ParamsWidget::mousePressEvent(QMouseEvent* event) {
        // QWidget::mousePressEvent(event);
    }

    void ParamsWidget::mouseReleaseEvent(QMouseEvent* event) {
        // QWidget::mouseReleaseEvent(event);
    }

    void ParamsWidget::resizeEvent(QResizeEvent* event) {
        // QWidget::resizeEvent(event);
    }

    void ParamsWidget::showEvent(QShowEvent* event) {
        // QWidget::showEvent(event);
    }
}
