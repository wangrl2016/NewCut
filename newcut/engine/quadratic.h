//
// Created by wangrl2016 on 2023/1/8.
//

#ifndef NEWCUT_QUADRATIC_H
#define NEWCUT_QUADRATIC_H

#include <boost/numeric/ublas/matrix.hpp>
#include "newcut/engine/vector.h"

namespace nc {
    // Class for generic linear and quadratic equation supports
    // translation and rotation of an equation.
    // 一元二次方程(y = a * x^2 + b * x + c, where a != 0)
    class Quadratic {
    public:
        explicit Quadratic();

        Quadratic(const Quadratic& quadratic);

        static VectorSolutions GetIntersection(const Quadratic& l1, const Quadratic& l2);

        friend std::ostream& operator<<(std::ostream& os, const Quadratic& l);
    private:
        boost::numeric::ublas::matrix<double> quad_;
        boost::numeric::ublas::vector<double> linear_;
        double const_;
        bool is_quadratic_;
        bool valid_;    // whether this quadratic form is valid
    };
}

#endif //NEWCUT_QUADRATIC_H
