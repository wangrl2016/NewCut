//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_COLOR_H
#define NEWCUT_COLOR_H

#include <QColor>
#include "newcut/engine/flags.h"

namespace nc {
    class Color : public QColor, public Flags {
    public:
        Color() : QColor(), Flags() {}

        Color(int r, int g, int b) : QColor(r, g, b), Flags() {}

        Color(int r, int g, int b, int a) : QColor(r, g, b, a), Flags() {}

        Color(const QColor& c) : QColor(c), Flags() {}

        Color(const Qt::GlobalColor color) : QColor(color), Flags() {}

        Color(const Color& c) : QColor(c), Flags() {
            set_flags(c.flags());
        }

        Color(uint32_t f) : QColor(), Flags(f) {}

        Color(QString name) : QColor(name), Flags() {}

        // A copy of this color is defined by layer.
        Color StripFlags() const {
            return Color(red(), green(), blue());
        }

    };
}

#endif //NEWCUT_COLOR_H
