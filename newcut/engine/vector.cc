//
// Created by wangrl2016 on 2022/12/28.
//

#include <cmath>
#include "newcut/engine/vector.h"

namespace nc {
    // Constructor for a point with given coordinates.
    Vector::Vector(double vx, double vy, double vz)
            : x(vx), y(vy), z(vz), valid(true) {}

    // Constructor for a unit vector with given angle
    Vector::Vector(double angle)
            : x(cos(angle)), y(sin(angle)), valid(true) {}

    Vector::Vector(bool valid) : valid(valid) {}

    Vector::operator bool() const { return valid; }

    void Vector::Set(double angle) {
        x = cos(angle);
        y = sin(angle);
        z = 0.0;
        valid = true;
    }

    void Vector::Set(double vx, double vy, double vz) {
        x = vx;
        y = vy;
        z = vz;
        valid = true;
    }

    void Vector::SetPolar(double radius, double angle) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        z = 0.0;
        valid = true;
    }

    Vector Vector::Polar(double rho, double theta) {
        return { rho * cos(theta), rho * sin(theta), 0.0f};
    }

    double Vector::Angle() const {
        return 0.0;
    }
}
