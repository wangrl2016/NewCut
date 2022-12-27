//
// Created by wr on 2022/12/28.
//

#ifndef NEWCUT_VECTOR_H
#define NEWCUT_VECTOR_H

namespace nc {
    // Represents a 3d vector (x/y/z)
    class Vector {
    public:
        Vector() = default;

        Vector(double vx, double vy, double vz = 0.0);

        explicit Vector(double angle);

        explicit Vector(bool valid);

        ~Vector() = default;

    public:
        double x_ = 0.0;
        double y_ = 0.0;
        double z_ = 0.0;
        bool valid_ = false;
    };
}


#endif //NEWCUT_VECTOR_H
