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
    Complex new_z1 = m_z0 * other.m_z1 - m_z1 * (~other.m_z0);

    return Quaternion(std::move(new_z0), std::move(new_z1));
}

// methods

Quaternion Quaternion::inverse() const {
    // TODO

    Complex new_z0 = Complex::Zero();
    Complex new_z1 = Complex::Zero();

    return Quaternion(std::move(new_z0), std::move(new_z1));
}

Quaternion Quaternion::pow(int n) const { // simple recursive binary pow
    if (n == 0)
        return Quaternion(1, 0, 0, 0);

    if (n == 1)
        return *this;

    if (n & 1)
        return (*this) * pow(n-1);
    
    Quaternion half_pow = pow(n / 2);

    return half_pow * half_pow;
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
