#pragma once // math/quaternion.hpp

#include "math/complex.hpp"
#include "math/vec3.hpp"

namespace iheay::math {

class Quaternion {
public:
    Quaternion(double a, double b, double c, double d);
    Quaternion(Complex z0, Complex z1);

    constexpr double a() const { return m_z0.real(); }
    constexpr double b() const { return m_z0.imag(); }
    constexpr double c() const { return m_z1.real(); }
    constexpr double d() const { return m_z1.imag(); }

    Complex z0() const;
    Complex z1() const;

    double modulus() const;

    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator-(const Quaternion& other) const;
    Quaternion operator*(const Quaternion& other) const;

    Quaternion operator-() const;
    Quaternion operator~() const; // conjugate

    /*Quaternion operator+=(const Quaternion& other) const;
    Quaternion operator-=(const Quaternion& other) const;
    Quaternion operator*=(const Quaternion& other) const;*/

    Quaternion inverse() const;
    Quaternion pow(int n) const;

    static Vec3 rotate_point(const Vec3& point, const Vec3& u, double phi);

    static Vec3 rotate_x(const Vec3& point, double phi);
    static Vec3 rotate_y(const Vec3& point, double phi);
    static Vec3 rotate_z(const Vec3& point, double phi);

private:
    Complex m_z0;
    Complex m_z1;
};

std::ostream& operator<<(std::ostream& os, const Quaternion& q);

} // namespace iheay::math
