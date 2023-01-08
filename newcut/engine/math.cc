//
// Created by wangrl2016 on 2022/12/30.
//

#include <cmath>
#include "newcut/engine/math.h"

namespace nc {
    int Math::Round(double v) {
        return (int) lrint(v);
    }

    double Math::Round(const double v, const double precision) {
        return precision * llround(v / precision);
    }

    double Math::Pow(double x, double y) {

        return ::pow(x, y);
    }
}
