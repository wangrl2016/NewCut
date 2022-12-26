//
// Created by wangrl2016 on 2022/12/23.
//

#include <QApplication>
#include <QHBoxLayout>
#include <QList>
#include "view/path_stroke/path_stroke_widget.h"

namespace nc {
    PathStrokeWidget::PathStrokeWidget(bool small_screen) {
        // Widget construction and property setting.
        renderer_ = new PathStrokeRenderer(this, small_screen);
        controls_ = new PathStrokeControls(nullptr, renderer_, small_screen);

        // Layouting
        auto* view_layout = new QHBoxLayout(this);
        view_layout->addWidget(renderer_);

        if (!small_screen) {
            view_layout->addWidget(controls_);
        }

        renderer_->LoadDescription(":html/path_stroke/path_stroke.html");

        // 使用类名加函数名
        connect(renderer_, &PathStrokeRenderer::Clicked,
                this, &PathStrokeWidget::ShowControls);
        connect(controls_, &PathStrokeControls::OkPressed,
                this, &PathStrokeWidget::HideControls);
        // 使用SIGNAL和SLOT宏
        connect(controls_, SIGNAL(QuitPressed()),
                QApplication::instance(), SLOT(quit()));
    }

    void PathStrokeWidget::ShowControls() {
        controls_->showFullScreen();
    }

    void PathStrokeWidget::HideControls() {
        controls_->hide();
    }

    void PathStrokeWidget::SetStyle(QStyle* style) {
        QWidget::setStyle(style);
        if (controls_ != nullptr) {
            controls_->setStyle(style);

            const QList<QWidget *> widgets = controls_->findChildren<QWidget *>();
            for (QWidget * w : widgets) {
                w->setStyle(style);
            }
        }
    }
}
