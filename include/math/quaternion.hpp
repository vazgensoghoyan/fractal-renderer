#pragma once // quaternion.hpp

#include "complex.hpp"
#include "vec3.hpp"

namespace iheay::math {

class Quaternion {
public:
    Quaternion(double a, double b, double c, double d);
    Quaternion(Complex z0, Complex z1);

    double get_a() const;
    double get_b() const;
    double get_c() const;
    double get_d() const;
    Complex get_z0() const;
    Complex get_z1() const;
    double get_modulus() const;

    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator-(const Quaternion& other) const;
    Quaternion operator*(const Quaternion& other) const;

    Quaternion operator-() const;
    Quaternion operator~() const; // conjugate 

    Quaternion inverse() const;
    Quaternion pow(int n) const;

    static Vec3 rotate_point(const Vec3& point, const Vec3& u, double phi);

    static Vec3 rotate_x(const Vec3& point, double phi);
    static Vec3 rotate_y(const Vec3& point, double phi);
    static Vec3 rotate_z(const Vec3& point, double phi);

    std::string to_string() const;

private:
    Complex m_z0;
    Complex m_z1;
};

} // namespace iheay::math
