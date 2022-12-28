//
// Created by wangrl2016 on 2022/12/28.
//

#include <cmath>
#include "newcut/engine/vector.h"

namespace nc {
    Vector::Vector(double vx, double vy, double vz)
            : x_(vx), y_(vy), z_(vz), valid_(true) {}

    Vector::Vector(double angle)
            : x_(cos(angle)), y_(sin(angle)), valid_(true) {}

    Vector::Vector(bool valid) : valid_(valid) {}

    Vector::operator bool() const { return valid_; }

    void Vector::Set(double angle) {
        x_ = cos(angle);
        y_ = sin(angle);
        z_ = 0.0;
        valid_ = true;
    }

    void Vector::Set(double vx, double vy, double vz) {
        x_ = vx;
        y_ = vy;
        z_ = vz;
        valid_ = true;
    }

}
