//
// Created by wangrl2016 on 2022/12/23.
//

#ifndef NEWCUT_PATH_STROKE_WIDGET_H
#define NEWCUT_PATH_STROKE_WIDGET_H

#include <QWidget>

#include "view/path_stroke/path_stroke_controls.h"
#include "view/path_stroke/path_stroke_renderer.h"

namespace nc {
    class PathStrokeWidget : public QWidget {
    Q_OBJECT
    public:
        PathStrokeWidget(bool small_screen);

        void SetStyle(QStyle* style);

    private slots:

        void ShowControls();

        void HideControls();

    private:
        PathStrokeRenderer* renderer_;
        PathStrokeControls* controls_;
    };
}

#endif //NEWCUT_PATH_STROKE_WIDGET_H
