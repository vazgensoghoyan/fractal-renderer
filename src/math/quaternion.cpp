#include "math/quaternion.hpp"

using namespace iheay::math;

// constructors

Quaternion::Quaternion(double a, double b, double c, double d)
    : z0_(Complex::Algebraic(a, b))
    , z1_(Complex::Algebraic(c, d))
{ }

Quaternion::Quaternion(Complex z0, Complex z1)
    : z0_(z0)
    , z1_(z1)
{ }

// property

double Quaternion::get_modulus() const {
    return std::hypot(z0_.get_modulus(), z1_.get_modulus());
}

// unary operators

Quaternion Quaternion::operator-() const {
    return Quaternion(-z0_, -z1_);
}

Quaternion Quaternion::operator~() const { // conjugate
    return Quaternion(~z0_, -z1_);
}

// binary operators

Quaternion Quaternion::operator+(const Quaternion& other) const {
    Complex new_z0 = z0_ + other.z0_;
    Complex new_z1 = z1_ + other.z1_;

    return Quaternion(std::move(new_z0), std::move(new_z1));
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
    return *this + (-other);
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    Complex new_z0 = z0_ * other.z0_ - z1_ * (~other.z1_);
    Complex new_z1 = z0_ * other.z1_ - z1_ * (~other.z0_);

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
        z0_.get_real(),
        z0_.get_imag(),
        z1_.get_real(),
        z1_.get_imag()
    );
}
