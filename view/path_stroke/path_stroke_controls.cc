//
// Created by wangrl2016 on 2022/12/23.
//

#include "view/path_stroke/path_stroke_controls.h"

namespace nc {
    PathStrokeControls::PathStrokeControls(QWidget* parent,
                                           nc::PathStrokeRenderer* renderer)
            : QWidget(parent), renderer_(renderer) {}

    void PathStrokeControls::EmitQuitSignal() {

    }

    void PathStrokeControls::EmitOkSignal() {

    }

    void PathStrokeControls::CreateCommonControls(QWidget* widget) {

    }

    void PathStrokeControls::LayoutForDesktop() {

    }
}
