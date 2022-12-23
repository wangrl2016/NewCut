//
// Created by wangrl2016 on 2022/12/23.
//

#include <QPainter>
#include <QPainterPath>
#include "view/path_stroke/path_stroke_renderer.h"

namespace nc {
    PathStrokeRenderer::PathStrokeRenderer(QWidget* parent, bool small_screen)
            : ArthurFrame(parent) {
        small_screen_ = small_screen;
        point_size_ = 10;
        active_point_ = -1;
        cap_style_ = Qt::FlatCap;
        join_style_ = Qt::BevelJoin;
        path_mode_ = kCurveMode;
        pen_width_ = 1;
        pen_style_ = Qt::SolidLine;
        was_animated_ = true;
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAttribute(Qt::WA_AcceptTouchEvents);
    }

    void PathStrokeRenderer::Paint(QPainter* painter) {
        if (points_.isEmpty())
            InitializePoints();

        painter->setRenderHint(QPainter::Antialiasing);

        QPalette pal = palette();
        painter->setPen(Qt::NoPen);

        // Construct the path.
        QPainterPath path;
        path.moveTo(points_.at(0));

        if (path_mode_ == kLineMode) {
            for (auto& point : points_)
                path.lineTo(point);
        } else {
            int i = 1;
            while (i + 2 < points_.size()) {
                path.cubicTo(points_.at(i),
                             points_.at(i + 1),
                             points_.at(i + 2));
                i += 3;
            }
            while (i < points_.size()) {
                path.lineTo( points_.at(i));
                i++;
            }
        }

        // Draw the path
        {
            QColor lg = Qt::red;
            // The "custom" pen
            if (pen_style_ == Qt::NoPen) {
                QPainterPathStroker stroker;
                stroker.setWidth(pen_width_);
                stroker.setJoinStyle(join_style_);
                stroker.setCapStyle(cap_style_);

                QList<qreal> dashes;
                qreal space = 4;
                dashes << 1 << space
                        << 3 << space
                        << 9 << space
                        << 27 << space
                        << 9 << space
                        << 3 << space;
                stroker.setDashPattern(dashes);
                QPainterPath stroke = stroker.createStroke(path);
                painter->fillPath(stroke, lg);
            } else {
                QPen pen(lg, pen_width_, pen_style_, cap_style_, join_style_);
                painter->strokePath(path, pen);
            }
        }

        {
            // Draw the control points
            painter->setPen(QColor(50, 100, 120, 200));
            painter->setBrush(QColor(200, 200, 210, 120));
            for (auto& pos : points_) {
                painter->drawEllipse(QRectF(pos.x() - point_size_,
                                            pos.y() - point_size_,
                                            point_size_ * 2, point_size_ * 2));
            }
            painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
            painter->setBrush(Qt::NoBrush);
            painter->drawPolyline(points_);
        }
    }

    void PathStrokeRenderer::mousePressEvent(QMouseEvent* event) {
        QWidget::mousePressEvent(event);
    }

    void PathStrokeRenderer::mouseMoveEvent(QMouseEvent* event) {
        QWidget::mouseMoveEvent(event);
    }

    void PathStrokeRenderer::mouseReleaseEvent(QMouseEvent* event) {
        QWidget::mouseReleaseEvent(event);
    }

    void PathStrokeRenderer::timerEvent(QTimerEvent* event) {
        QObject::timerEvent(event);
    }

    bool PathStrokeRenderer::event(QEvent* event) {
        return QWidget::event(event);
    }

    void PathStrokeRenderer::set_animation(bool animation) {

    }

    void PathStrokeRenderer::InitializePoints() {
        const int count = 7;
        points_.clear();
        vectors_.clear();

        QTransform m;
        qreal rot = 360.0 / count;
        QPointF center(width() / 2, height() / 2);
        QTransform vm;
        vm.shear(2, -1);
        vm.scale(3, 3);

        for (int i = 0; i < count; i++) {
            vectors_ << QPointF(0.1f, 0.25f) * (m * vm);
            points_ << QPointF(0, 100) * m + center;
            m.rotate(rot);
        }
    }

    void PathStrokeRenderer::UpdatePoints() {

    }
}
