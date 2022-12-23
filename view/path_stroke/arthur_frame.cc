//
// Created by admin on 2022/12/23.
//

#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include "view/path_stroke/arthur_frame.h"

namespace nc {
    ArthurFrame::ArthurFrame(QWidget* parent)
            : QWidget(parent), tile_(QPixmap(128, 128)) {
        tile_.fill(Qt::white);
        QPainter painter(&tile_);
        QColor color(230, 230, 230);
        painter.fillRect(0, 0, 64, 64, color);
        painter.fillRect(64, 64, 64, 64, color);
        painter.end();
    }

    void ArthurFrame::paintEvent(QPaintEvent* event) {
        static QImage* static_image = nullptr;

        QPainter painter;

        if (prefer_image()) {

        } else {
            painter.begin(this);
        }

        painter.setClipRect(event->rect());
        painter.setRenderHint(QPainter::Antialiasing);

        QPainterPath clip_path;

        QRect r = rect();
        qreal left = r.x() + 1;
        qreal top = r.y() + 1;
        qreal right = r.right();
        qreal bottom = r.bottom();
        qreal radius2 = 8 * 2;

        clip_path.moveTo(right - radius2, top);
        clip_path.arcTo(right - radius2, top, radius2, radius2, 90, -90);
        clip_path.arcTo(right - radius2, bottom - radius2, radius2, radius2, 0, -90);
        clip_path.arcTo(left, bottom - radius2, radius2, radius2, 270, -90);
        clip_path.arcTo(left, top, radius2, radius2, 180, -90);
        clip_path.closeSubpath();

        // 保存此时QPainter对象的状态
        painter.save();
        painter.setClipPath(clip_path, Qt::IntersectClip);

        painter.drawTiledPixmap(rect(), tile_);

        // client painting
        Paint(&painter);

        // 恢复QPainter对象为之前保存的状态
        painter.restore();

        painter.save();

        int level = 180;
        painter.setPen(QPen(QColor(level, level, level), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(clip_path);
    }

    void ArthurFrame::resizeEvent(QResizeEvent* event) {
        QWidget::resizeEvent(event);
    }
}
