//
// Created by wangrl2016 on 2022/12/23.
//

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPixmapCache>
#include "view/path_stroke/arthur_style.h"

namespace nc {
    // 图像缓存
    QPixmap cached(const QString& img) {
        QPixmap pm;
        if (QPixmapCache::find(img, &pm))
            return pm;

        pm = QPixmap::fromImage(QImage(img),
                                Qt::OrderedDither | Qt::OrderedAlphaDither);
        if (pm.isNull())
            return {};

        QPixmapCache::insert(img, pm);
        return pm;
    }

    ArthurStyle::ArthurStyle() : QCommonStyle() {

    }

    void ArthurStyle::DrawHoverRect(QPainter* painter,
                                    const QRect& rect) const {
        qreal h = rect.height();
        qreal h2 = rect.height() / qreal (2);
        QPainterPath path;
        path.addRect(rect.x() + h2, rect.y() + 0, rect.width() - h2 * 2, rect.height());
        path.addEllipse(rect.x(), rect.y(), h, h);
        path.setFillRule(Qt::WindingFill);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(191, 215, 191));   // rgba
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawPath(path);
    }

    void ArthurStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                                    const QWidget* widget) const {
        Q_ASSERT(option);
        switch (element) {
            case QStyle::PE_FrameFocusRect:
                break;
            case QStyle::PE_IndicatorRadioButton:

            default:
                QCommonStyle::drawPrimitive(element, option, painter, widget);
                break;
        }
    }

    void ArthurStyle::drawControl(QStyle::ControlElement element, const QStyleOption* opt, QPainter* p,
                                  const QWidget* w) const {
        QCommonStyle::drawControl(element, opt, p, w);
    }

    void ArthurStyle::drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p,
                                         const QWidget* w) const {
        QCommonStyle::drawComplexControl(cc, opt, p, w);
    }

    QSize ArthurStyle::sizeFromContents(QStyle::ContentsType ct, const QStyleOption* opt, const QSize& contentsSize,
                                        const QWidget* widget) const {
        return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
    }

    QRect ArthurStyle::subElementRect(QStyle::SubElement r, const QStyleOption* opt, const QWidget* widget) const {
        return QCommonStyle::subElementRect(r, opt, widget);
    }

    QRect ArthurStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex* opt, QStyle::SubControl sc,
                                      const QWidget* w) const {
        return QCommonStyle::subControlRect(cc, opt, sc, w);
    }

    int ArthurStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption* opt, const QWidget* widget) const {
        return QCommonStyle::pixelMetric(m, opt, widget);
    }

    void ArthurStyle::polish(QPalette& palette) {
        QCommonStyle::polish(palette);
    }

    void ArthurStyle::polish(QWidget* widget) {
        QCommonStyle::polish(widget);
    }

    void ArthurStyle::unpolish(QWidget* widget) {
        QCommonStyle::unpolish(widget);
    }
}
