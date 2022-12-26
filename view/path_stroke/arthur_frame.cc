//
// Created by admin on 2022/12/23.
//

#include <QAbstractTextDocumentLayout>
#include <QFile>
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

    void ArthurFrame::SetDescriptionEnabled(bool enabled) {
        if (show_doc_ != enabled) {
            show_doc_ = enabled;
            emit DescriptionEnabledChanged(show_doc_);
            update();
        }
    }

    void ArthurFrame::LoadDescription(const QString& filename) {
        QFile text_file(filename);
        QString text;
        if (!text_file.open(QFile::ReadOnly)) {
            text = QString("Unable to load resource file: '%1'").arg(filename);
        } else {
            text = text_file.readAll();
        }
        SetDescription(text);
    }

    void ArthurFrame::SetDescription(const QString& html_desc) {
        document_ = new QTextDocument(this);
        document_->setHtml(html_desc);
    }

    void ArthurFrame::PaintDescription(QPainter* painter) {
        if (!document_)
            return;

        int page_width = qMax(width() - 100, 100);
        int page_height = qMax(height() - 100, 100);
        if (page_width != document_->pageSize().width()) {
            document_->setPageSize(QSize(page_width, page_height));
        }

        QRect text_rect(width() / 2 - page_width / 2,
                        height() / 2 - page_height / 2,
                        page_width,
                        page_height);
        int pad = 10;
        QRect clear_rect = text_rect.adjusted(-pad, -pad, pad, pad);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 0, 0, 63));
        int shade = 10;
        painter->drawRect(clear_rect.x() + clear_rect.width() + 1,
                          clear_rect.y() + shade,
                          shade,
                          clear_rect.height() + 1);
        painter->drawRect(clear_rect.x() + shade,
                          clear_rect.y() + clear_rect.height() + 1,
                          clear_rect.width() - shade + 1,
                          shade);
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setBrush(QColor(255, 255, 255, 220));
        painter->setPen(Qt::black);
        painter->drawRect(clear_rect);

        painter->setClipRegion(text_rect, Qt::IntersectClip);
        painter->translate(text_rect.topLeft());

        QAbstractTextDocumentLayout::PaintContext ctx;
        QLinearGradient g(0, 0, 0, text_rect.height());
        g.setColorAt(0, Qt::black);
        g.setColorAt(0.9, Qt::black);
        g.setColorAt(1, Qt::transparent);

        QPalette pal = palette();
        pal.setBrush(QPalette::Text, g);

        ctx.palette = pal;
        ctx.clip = QRect(0, 0, text_rect.width(), text_rect.height());
        document_->documentLayout()->draw(painter, ctx);
    }
}
