//
// Created by wangrl2016 on 2022/12/23.
//

#include <QRadioButton>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include "view/path_stroke/path_stroke_controls.h"

namespace nc {
    PathStrokeControls::PathStrokeControls(QWidget* parent,
                                           nc::PathStrokeRenderer* renderer,
                                           bool small_screen)
            : QWidget(parent), renderer_(renderer) {
        if (!small_screen) {
            LayoutForDesktop();
        }
    }

    void PathStrokeControls::EmitQuitSignal() {

    }

    void PathStrokeControls::EmitOkSignal() {

    }

    void PathStrokeControls::CreateCommonControls(QWidget* parent) {
        cap_group_ = new QGroupBox(parent);
        cap_group_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        auto* flat_cap = new QRadioButton(cap_group_);
        auto* square_cap = new QRadioButton(cap_group_);
        auto* round_cap = new QRadioButton(cap_group_);
        cap_group_->setTitle(tr("Cap Style"));
        flat_cap->setText(tr("Flat"));
        square_cap->setText(tr("Square"));
        round_cap->setText(tr("Round"));
        flat_cap->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        square_cap->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        round_cap->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        join_group_ = new QGroupBox(parent);
        join_group_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        auto* bevel_join = new QRadioButton(join_group_);
        auto* miter_join = new QRadioButton(join_group_);
        auto* svg_miter_join = new QRadioButton(join_group_);
        auto* round_join = new QRadioButton(join_group_);
        join_group_->setTitle(tr("Join Style"));
        bevel_join->setText(tr("Bevel"));
        miter_join->setText(tr("Miter"));
        svg_miter_join->setText(tr("SvgMiter"));
        round_join->setText(tr("Round"));

        auto* cap_group_layout = new QVBoxLayout(cap_group_);
        cap_group_layout->addWidget(flat_cap);
        cap_group_layout->addWidget(square_cap);
        cap_group_layout->addWidget(round_cap);

        auto* join_group_layout = new QVBoxLayout(join_group_);
        join_group_layout->addWidget(bevel_join);
        join_group_layout->addWidget(miter_join);
        join_group_layout->addWidget(svg_miter_join);
        join_group_layout->addWidget(round_join);

        flat_cap->setChecked(true);
        bevel_join->setChecked(true);
    }

    void PathStrokeControls::LayoutForDesktop() {
        auto* main_group = new QGroupBox(this);
        main_group->setFixedWidth(180);
        main_group->setTitle(tr("Path Stroking"));

        CreateCommonControls(main_group);

        auto* pen_width_group = new QGroupBox(main_group);
        auto* pen_width = new QSlider(Qt::Horizontal, pen_width_group);
        pen_width->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        pen_width_group->setTitle(tr("Pen Width"));
        pen_width->setRange(0, 500);

        auto* animated = new QPushButton(main_group);
        animated->setText(tr("Animate"));
        animated->setChecked(true);

        auto* show_source_button = new QPushButton(main_group);
        show_source_button->setText(tr("Show Source"));
        auto* whats_this_button = new QPushButton(main_group);
        whats_this_button->setText(tr("What's This?"));
        whats_this_button->setCheckable(true);

        // layouts
        auto* pen_width_layout = new QVBoxLayout(pen_width_group);
        pen_width_layout->addWidget(pen_width);

        // 此组件的布局
        auto* main_layout = new QVBoxLayout(this);
        main_layout->setContentsMargins(QMargins());
        main_layout->addWidget(main_group);

        auto* main_group_layout = new QVBoxLayout(main_group);
        main_group_layout->setContentsMargins(3, 3, 3, 3);
        main_group_layout->addWidget(cap_group_);
        main_group_layout->addWidget(join_group_);
        main_group_layout->addWidget(pen_width_group);
        main_group_layout->addWidget(animated);
        main_group_layout->addStretch(1);
        main_group_layout->addWidget(show_source_button);
        main_group_layout->addWidget(whats_this_button);
    }
}
