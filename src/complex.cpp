#include "complex.hpp"

// utils

double normalize_arg(double arg) { // to [0, 2pi)
    double normalized_arg = std::fmod(arg, 2 * M_PI);
    if (normalized_arg < 0) {
        normalized_arg += 2 * M_PI;
    }
    return normalized_arg;
}

// private empty constructor

Complex::Complex(double real, double imag, double modulus, double arg)
    : real_(real)
    , imag_(imag)
    , modulus_(modulus)
    , arg_(arg)
{
    if (modulus_ == 0)
        arg_ = 0;
}

// little public fabric

Complex Complex::Algebraic(double real, double imag) {
    double modulus = sqrt(real * real + imag * imag);

    double arg = atan2(imag, real);

    if (arg < 0) { // for arg to be in [0, 2pi)
        arg += 2 * M_PI;
    }
    
    return Complex(real, imag, modulus, arg);
}

Complex Complex::Trigonometric(double modulus, double arg) {
    if (modulus < 0) {
        throw std::domain_error("Modulus must be non-negative");
    }

    double normalized_arg = normalize_arg(arg);

    double real = modulus * cos(normalized_arg);
    double imag = modulus * sin(normalized_arg);

    return Complex(real, imag, modulus, normalized_arg);
}

// unary operators

Complex Complex::operator-() const {
    double new_arg = arg_ - M_PI; 
    if (new_arg < 0) {
        new_arg += 2 * M_PI;
    }

    return Complex(-real_, -imag_, modulus_, new_arg);
}

Complex Complex::operator~() const { // conjugate
    double new_arg = normalize_arg(M_PI - arg_);

    return Complex(real_, -imag_, modulus_, new_arg);
}

// binary operators

Complex Complex::operator+(const Complex& other) const {
    double new_real = real_ + other.real_;
    double new_imag = imag_ + other.imag_;

    return Complex::Algebraic(new_real, new_imag);
}

Complex Complex::operator-(const Complex& other) const {
    return *this + (-other);
}

Complex Complex::operator*(const Complex& other) const {
    double new_modulus = modulus_ * other.modulus_;
    double new_arg = arg_ + other.arg_;

    return Complex::Trigonometric(new_modulus, new_arg);
}

Complex Complex::operator/(const Complex& other) const {
    double new_modulus = modulus_ / other.modulus_;
    double new_arg = arg_ - other.arg_;

    return Complex::Trigonometric(new_modulus, new_arg);
}

// methods


