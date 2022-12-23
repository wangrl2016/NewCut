//
// Created by wangrl2016 on 2022/12/23.
//

#ifndef NEWCUT_PATH_STROKE_RENDERER_H
#define NEWCUT_PATH_STROKE_RENDERER_H

#include <QBasicTimer>
#include "view/path_stroke/arthur_frame.h"

namespace nc {
    class PathStrokeRenderer : public ArthurFrame {
    Q_OBJECT
        Q_PROPERTY(bool animation_ READ animation WRITE set_animation)
        Q_PROPERTY(qreal pen_width_ READ pen_width WRITE set_pen_width)
    public:
        enum PathMode {
            kCurveMode,
            kLineMode
        };

        explicit PathStrokeRenderer(QWidget* parent, bool small_screen = false);

        void Paint(QPainter*) override;

        void mousePressEvent(QMouseEvent* event) override;

        void mouseMoveEvent(QMouseEvent* event) override;

        void mouseReleaseEvent(QMouseEvent* event) override;

        void timerEvent(QTimerEvent* event) override;

        bool event(QEvent* event) override;

        QSize sizeHint() const override {
            return {500, 500};
        }

        bool animation() const {
            return timer_.isActive();
        }

        qreal pen_width() const {
            return pen_width_;
        }

        void set_pen_width(qreal width) {
            pen_width_ = width;
            update();
        }

    public slots:

        void set_animation(bool animation);

    private:
        void InitializePoints();

        void UpdatePoints();

    private:
        QBasicTimer timer_;

        PathMode path_mode_;

        bool was_animated_;

        qreal pen_width_;
        int point_count_;
        int point_size_;
        int active_point_;
        QList<QPointF> points_;
        QList<QPointF> vectors_;

        Qt::PenJoinStyle join_style_;
        Qt::PenCapStyle cap_style_;
        Qt::PenStyle pen_style_;

        bool small_screen_;
        QPoint mouse_press_;
        bool mouse_drag_;

        QHash<int, int> finger_point_mapping_;
    };
}

#endif //NEWCUT_PATH_STROKE_RENDERER_H