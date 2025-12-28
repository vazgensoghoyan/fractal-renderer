#include "math/quaternion.hpp"

using namespace iheay::math;

// constructors

Quaternion::Quaternion(double a, double b, double c, double d)
    : m_z0(Complex::Algebraic(a, b))
    , m_z1(Complex::Algebraic(c, d))
{ }

Quaternion::Quaternion(Complex z0, Complex z1)
    : m_z0(z0)
    , m_z1(z1)
{ }

// non-constexpr properties

Complex Quaternion::z0() const {
    return m_z0;
}

Complex Quaternion::z1() const {
    return m_z1;
}

double Quaternion::modulus() const {
    return std::hypot(m_z0.modulus(), m_z1.modulus());
}

// unary operators

Quaternion Quaternion::operator-() const {
    return Quaternion(-m_z0, -m_z1);
}

Quaternion Quaternion::operator~() const { // conjugate
    return Quaternion(~m_z0, -m_z1);
}

// binary operators

Quaternion Quaternion::operator+(const Quaternion& other) const {
    Complex new_z0 = m_z0 + other.m_z0;
    Complex new_z1 = m_z1 + other.m_z1;

    return Quaternion(std::move(new_z0), std::move(new_z1));
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
    return *this + (-other);
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    Complex new_z0 = m_z0 * other.m_z0 - m_z1 * (~other.m_z1);
    Complex new_z1 = m_z0 * other.m_z1 + m_z1 * (~other.m_z0);

    return Quaternion(std::move(new_z0), std::move(new_z1));
}

// += like operators
// TODO

/*Quaternion Quaternion::operator+=(const Quaternion& other) const {
    *this = *this + other;
    return *this;
}

Quaternion Quaternion::operator-=(const Quaternion& other) const {
    *this = *this - other;
    return *this;
}

Quaternion Quaternion::operator*=(const Quaternion& other) const {
    *this = *this * other;
    return *this;
}*/

// methods

Quaternion Quaternion::inverse() const {
    double modul = modulus();
    double squared = modul * modul;

    Complex new_z0 = ~m_z0 / squared;
    Complex new_z1 = -m_z1 / squared;

    return Quaternion(std::move(new_z0), std::move(new_z1));
}

Quaternion Quaternion::pow(int n) const {
    Quaternion result(1,0,0,0);
    Quaternion base = *this;

    while (n > 0) {
        if (n & 1) result = result * base;
        base = base * base;
        n >>= 1;
    }

    return result;
}

// static method to rotate

Vec3 Quaternion::rotate_point(const Vec3& point, const Vec3& u, double phi) {
    phi /= 2;
    
    Quaternion v = Quaternion( 0, point.x(), point.y(), point.z() );

    Vec3 s = u.normalized() * sin(phi);
    Quaternion q = Quaternion( cos(phi), s.x(), s.y(), s.z() );

    Quaternion answer = q * v * q.inverse();

    return Vec3( answer.b(), answer.c(), answer.d() );
}

Vec3 Quaternion::rotate_x(const Vec3& point, double phi) {
    return rotate_point(point, {1, 0, 0}, phi);
}

Vec3 Quaternion::rotate_y(const Vec3& point, double phi) {
    return rotate_point(point, {0, 1, 0}, phi);
}

Vec3 Quaternion::rotate_z(const Vec3& point, double phi) {
    return rotate_point(point, {0, 0, 1}, phi);
}

// printing

std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    return os << std::format(
        "{} + i * {} + j * {} + k * {}",
        q.a(),
        q.b(),
        q.c(),
        q.d()
    );
}
