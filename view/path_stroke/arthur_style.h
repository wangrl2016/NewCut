//
// Created by wangrl2016 on 2022/12/23.
//

#ifndef NEWCUT_ARTHUR_STYLE_H
#define NEWCUT_ARTHUR_STYLE_H

#include <QCommonStyle>

namespace nc {
    class ArthurStyle : public QCommonStyle {
    public:
        ArthurStyle();

        void DrawHoverRect(QPainter* painter,
                           const QRect& rect) const;

        void drawPrimitive(PrimitiveElement element,
                           const QStyleOption* option,
                           QPainter* painter,
                           const QWidget* widget = nullptr) const override;

        void drawControl(ControlElement element,
                         const QStyleOption* opt,
                         QPainter* p,
                         const QWidget* w = nullptr) const override;

        void drawComplexControl(ComplexControl cc,
                                const QStyleOptionComplex* opt,
                                QPainter* p,
                                const QWidget* w = nullptr) const override;

        QSize sizeFromContents(ContentsType ct,
                               const QStyleOption* opt,
                               const QSize& contentsSize,
                               const QWidget* widget = nullptr) const override;

        QRect subElementRect(SubElement r,
                             const QStyleOption* opt,
                             const QWidget* widget = nullptr) const override;

        QRect subControlRect(ComplexControl cc,
                             const QStyleOptionComplex* opt,
                             SubControl sc,
                             const QWidget* w = nullptr) const override;

        int pixelMetric(PixelMetric m,
                        const QStyleOption* opt = nullptr,
                        const QWidget* widget = nullptr) const override;

        void polish(QPalette& palette) override;

        void polish(QWidget* widget) override;

        void unpolish(QWidget* widget) override;
    };
}

#endif //NEWCUT_ARTHUR_STYLE_H
