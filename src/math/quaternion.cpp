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

// property

double Quaternion::get_a() const {
    return m_z0.get_real();
}

double Quaternion::get_b() const {
    return m_z0.get_imag();
}

double Quaternion::get_c() const {
    return m_z1.get_real();
}

double Quaternion::get_d() const {
    return m_z1.get_imag();
}

Complex Quaternion::get_z0() const {
    return m_z0;
}

Complex Quaternion::get_z1() const {
    return m_z1;
}

double Quaternion::get_modulus() const {
    return std::hypot(m_z0.get_modulus(), m_z1.get_modulus());
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

// methods

Quaternion Quaternion::inverse() const {
    double modulus = get_modulus();
    double squared = modulus * modulus;

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
    
    Quaternion v = Quaternion( 0, point.get_x(), point.get_y(), point.get_z() );

    Vec3 s = u.normalize() * sin(phi);
    Quaternion q = Quaternion( cos(phi), s.get_x(), s.get_y(), s.get_z() );

    Quaternion answer = q * v * q.inverse();

    return Vec3( answer.get_b(), answer.get_c(), answer.get_d() );
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

// to string

std::string Quaternion::to_string() const {
    return std::format(
        "{} + i * {} + j * {} + k * {}",
        m_z0.get_real(),
        m_z0.get_imag(),
        m_z1.get_real(),
        m_z1.get_imag()
    );
}
