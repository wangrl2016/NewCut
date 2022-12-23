//
// Created by wangrl2016 on 2022/12/23.
//

#ifndef NEWCUT_PATH_STROKE_CONTROLS_H
#define NEWCUT_PATH_STROKE_CONTROLS_H

#include <QWidget>
#include <QGroupBox>
#include "view/path_stroke/path_stroke_renderer.h"

namespace nc {
    class PathStrokeControls : public QWidget {
    Q_OBJECT
    public:
        PathStrokeControls(QWidget* parent,
                           PathStrokeRenderer* renderer);

    signals:

        void OkPressed();

        void QuitPressed();

    private slots:

        void EmitQuitSignal();

        void EmitOkSignal();

    private:
        void CreateCommonControls(QWidget* widget);

        void LayoutForDesktop();

    private:
        PathStrokeRenderer* renderer_;

        QGroupBox* cap_group_;
        QGroupBox* join_group_;
        QGroupBox* style_group_;
        QGroupBox* path_mode_group_;
    };
}

#endif //NEWCUT_PATH_STROKE_CONTROLS_H
