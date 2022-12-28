//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_VECTOR_H
#define NEWCUT_VECTOR_H

#include <ostream>

namespace nc {
    // Represents a 3d vector (x/y/z)
    class Vector {
    public:
        Vector() = default;

        Vector(double vx, double vy, double vz = 0.0);

        explicit Vector(double angle);

        explicit Vector(bool valid);

        ~Vector() = default;

        explicit operator bool() const;

        // Set to unit vector by the direction of angle
        void Set(double angle);

        void Set(double vx, double vy, double vz = 0.0);

        void SetPolar(double radius, double angle);

        static Vector Polar(double rho, double theta);

        double DistanceTo(const Vector& v) const;

        double Angle() const;

        double AngleTo(const Vector& v) const;

        double AngleBetween(const Vector& v1, const Vector& v2) const;

        double Magnitude() const;

        double Squared() const;

        double SquaredTo(const Vector& v1) const;

        Vector Lerp(const Vector& v, double t) const;

        bool IsInWindow(const Vector& first_cornel, const Vector& second_cornel) const;

        bool IsInWindowOrdered(const Vector& v_low, const Vector& v_height) const;

        Vector ToInteger();

        Vector Move(const Vector& offset);

        Vector Rotate(double ang);

        Vector Rotate(const Vector& angle_vector);

        Vector Rotate(const Vector& center, double angle);

        Vector Rotate(const Vector& center, const Vector& angle_vector);

        Vector Scale(double factor);

        Vector Scale(const Vector& factor);

        Vector Scale(const Vector& factor) const;

        Vector Scale(const Vector& center, const Vector& factor);

        Vector Mirror(const Vector& axis_point1, const Vector& axis_point2);

        double DotP(const Vector& v1) const;

        Vector operator+(const Vector& v) const;

        Vector operator+(double d) const;

        Vector operator-(const Vector& v) const;

        Vector operator-(double d) const;

        Vector operator*(const Vector& v) const;

        Vector operator/(const Vector& v) const;

        Vector operator*(double s) const;

        Vector operator/(double s) const;

        Vector operator-() const;

        Vector operator+=(const Vector& v);

        Vector operator-=(const Vector& v);

        Vector operator*=(const Vector& v);

        Vector operator/=(const Vector& v);

        Vector operator*=(double s);

        Vector operator/=(double s);

        bool operator==(const Vector& v) const;

        bool operator!=(const Vector& v) const;

        bool operator==(bool valid) const;

        bool operator!=(bool valid) const;

        static Vector Minimum(const Vector& v1, const Vector& v2);

        static Vector Maximum(const Vector& v1, const Vector& v2);

        static Vector CrossP(const Vector& v1, const Vector& v2);

        static double DotP(const Vector& v1, const Vector v2);

        static double PosInLine(const Vector& start,
                                const Vector& end,
                                const Vector& pos);

        // Switch x, y for all vector.
        Vector FlipXY() const;

        friend std::ostream& operator<<(std::ostream&, const Vector& v);

    public:
        double x_ = 0.0;
        double y_ = 0.0;
        double z_ = 0.0;
        bool valid_ = false;
    };
}

#endif //NEWCUT_VECTOR_H
