#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "complex.hpp"

namespace iheay::math {

    class Quaternion {

    public:
        Quaternion(double a, double b, double c, double d);
        Quaternion(Complex z0, Complex z1);

        double get_modulus() const;

        Quaternion operator+(const Quaternion& other) const;
        Quaternion operator-(const Quaternion& other) const;
        Quaternion operator*(const Quaternion& other) const;

        Quaternion operator-() const;
        Quaternion operator~() const; // conjugate 

        Quaternion inverse() const;
        Quaternion pow(int n) const;

        std::string to_string() const;

    private:

    private:
        Complex z0_;
        Complex z1_;

    };

}

#endif // QUATERNION_HPP
