//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_MATH_H
#define NEWCUT_MATH_H

#include <boost/math/special_functions/ellint_2.hpp>

namespace nc {
    class Vector;

    class Math {
    private:
        Math() = delete;

    public:
        static int Round(double v);

        static double Round(const double v, const double precision);

        static double Pow(double x, double y);

        static Vector Pow(Vector x, double y);

        static bool Equal(const double d1, const double d2);

        static double Rad2deg(double a);

        static double Deg2Rad(double a);
    };
}

#endif //NEWCUT_MATH_H
