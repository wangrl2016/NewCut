//
// Created by wangrl2016 on 2022/12/23.
//

#include <glog/logging.h>
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
        painter->setPen(Qt::NoPen);

        // Construct the path.
        QPainterPath path;
        path.moveTo(points_.at(0));

        if (path_mode_ == kLineMode) {
            for (auto& point: points_)
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
                path.lineTo(points_.at(i));
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
            for (auto& pos: points_) {
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
        LOG(INFO) << __FUNCTION__;
        if (!finger_point_mapping_.isEmpty())
            return;
        SetDescriptionEnabled(false);
        active_point_ = -1;
        qreal distance = -1;
        for (int i = 0; i < points_.size(); i++) {
            qreal d = QLineF(event->position().toPoint(), points_.at(i)).length();
            if ((distance < 0 && d < 8 * point_size_) || d < distance) {
                distance = d;
                active_point_ = i;
            }
        }

        if (active_point_ != -1) {
            was_animated_ = timer_.isActive();
            set_animation(false);
            mouseMoveEvent(event);
        }

        mouse_drag_ = !small_screen_;
        mouse_press_ = event->position().toPoint();
    }

    void PathStrokeRenderer::mouseMoveEvent(QMouseEvent* event) {
        if (!finger_point_mapping_.isEmpty()) {
            return;
        }
        // If we've moved more than 25 pixels, assume user is dragging.
        if (!mouse_drag_ && QPoint(mouse_press_ - event->position().toPoint()).manhattanLength() > 25) {
            mouse_drag_ = true;
        }

        if (mouse_drag_ && active_point_ >= 0 && active_point_ < points_.size()) {
            points_[active_point_] = event->position().toPoint();
            update();
        }
    }

    void PathStrokeRenderer::mouseReleaseEvent(QMouseEvent* event) {
        if (!finger_point_mapping_.isEmpty())
            return;
        active_point_ = -1;
        set_animation(was_animated_);

        if (!mouse_drag_ && small_screen_)
                emit Clicked();
    }

    void PathStrokeRenderer::timerEvent(QTimerEvent* event) {
        if (event->timerId() == timer_.timerId()) {
            UpdatePoints();
        }
    }

    bool PathStrokeRenderer::event(QEvent* event) {
        bool touch_begin = false;
        switch (event->type()) {
            case QEvent::TouchBegin:
                touch_begin = true;
                Q_FALLTHROUGH();
            case QEvent::TouchUpdate: {
                const auto* const touch_event = dynamic_cast<const QTouchEvent*>(event);
                const auto points = touch_event->points();
                for (const auto& point: points) {
                    const int id = point.id();
                    switch (point.state()) {
                        case QEventPoint::Pressed: {
                            // find the point, move it
                            const auto mapped_points = finger_point_mapping_.values();
                            QSet<int> active_points = QSet<int>(mapped_points.begin(), mapped_points.end());
                            int active_point = -1;
                            qreal distance = -1;
                            auto points_count = points_.size();
                            for (int i = 0; i < points_count; ++i) {
                                if (active_points.contains(i))
                                    continue;

                                qreal d = QLineF(point.position(), points_.at(i)).length();
                                if ((distance < 0 && d < 12 * point_size_) || d < distance) {
                                    distance = d;
                                    active_point = i;
                                }
                            }
                            if (active_point != -1) {
                                finger_point_mapping_.insert(point.id(), active_point);
                                points_[active_point] = point.position();
                            }
                            break;
                        }
                        case QEventPoint::Released: {
                            // move the point and release
                            QHash<int, int>::iterator it = finger_point_mapping_.find(id);
                            points_[it.value()] = point.position();
                            finger_point_mapping_.erase(it);
                            break;
                        }
                        case QEventPoint::Updated: {
                            // move the point
                            const int pointIdx = finger_point_mapping_.value(id, -1);
                            if (pointIdx >= 0)
                                points_[pointIdx] = point.position();
                            break;
                        }
                        default:
                            break;
                    }
                }
                if (finger_point_mapping_.isEmpty()) {
                    event->ignore();
                    return false;
                } else {
                    if (touch_begin) {
                        was_animated_ = timer_.isActive();
                        set_animation(false);
                    }
                    update();
                    return true;
                }
            }
            case QEvent::TouchEnd:
                if (finger_point_mapping_.isEmpty()) {
                    event->ignore();
                    return false;
                }
                finger_point_mapping_.clear();
                set_animation(was_animated_);
                return true;
            default:
                break;
        }
        return QWidget::event(event);
    }

    void PathStrokeRenderer::set_animation(bool animation) {
        timer_.stop();
        if (animation) {
            timer_.start(25, this);
        }
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
        qreal pad = 10;
        qreal left = pad;
        qreal right = width() - pad;
        qreal top = pad;
        qreal bottom = height() - pad;

        Q_ASSERT(points_.size() == vectors_.size());
        for (int i = 0; i < points_.size(); i++) {
            QPointF pos = points_.at(i);
            QPointF vec = vectors_.at(i);
            pos += vec;
            if (pos.x() < left || pos.x() > right) {
                vec.setX(-vec.x());
                pos.setX(pos.x() < left ? left : right);
            }
            if (pos.y() < top || pos.y() > bottom) {
                vec.setY(-vec.y());
                pos.setY(pos.y() < top ? top : bottom);
            }
            points_[i] = pos;
            vectors_[i] = vec;
        }
        update();
    }
}
