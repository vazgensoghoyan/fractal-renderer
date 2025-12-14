#include "complex.hpp"

// utils

static const double PI = acos(-1.0);

double normalize_arg(double arg) { // to [0, 2pi)
    double res = std::fmod(arg, 2 * PI);

    if (res < 0) res += 2 * PI;

    return res;
}

// private empty constructor

Complex::Complex(double real, double imag) : real_(real), imag_(imag)
{ }

// little public fabric

Complex Complex::Algebraic(double real, double imag) {
    return Complex(real, imag);
}

Complex Complex::Trigonometric(double modulus, double arg) {
    if (modulus < 0) {
        throw std::domain_error("Modulus must be non-negative");
    }

    double norm_arg = normalize_arg(arg);

    double real = modulus * cos(norm_arg);
    double imag = modulus * sin(norm_arg);

    return Complex(real, imag);
}

// properties

double Complex::get_modulus() const {
    return std::hypot(real_, imag_);
}

double Complex::get_arg() const {
    double arg = atan2(imag_, real_);
    return normalize_arg(arg);
}

// unary operators

Complex Complex::operator-() const {
    return Complex(-real_, -imag_);
}

Complex Complex::operator~() const { // conjugate
    return Complex(real_, -imag_);
}

// binary operators

Complex Complex::operator+(const Complex& other) const {
    double new_real = real_ + other.real_;
    double new_imag = imag_ + other.imag_;

    return Algebraic(new_real, new_imag);
}

Complex Complex::operator-(const Complex& other) const {
    return *this + (-other);
}

Complex Complex::operator*(const Complex& other) const {
    double new_modulus = get_modulus() * other.get_modulus();
    double new_arg = get_arg() + other.get_arg();

    return Trigonometric(new_modulus, new_arg);
}

Complex Complex::operator/(const Complex& other) const {
    if (other.get_modulus() == 0)
        throw std::runtime_error("Cant divide by complex zero");

    double new_modulus = get_modulus() / other.get_modulus();
    double new_arg = get_arg() - other.get_arg();

    return Trigonometric(new_modulus, new_arg);
}

// methods

std::vector<Complex> Complex::get_roots(int n) const {
    // TODO
    return {};
}

Complex Complex::pow(int n) const { // simple recursive binary pow
    double new_modulus = std::pow(get_modulus(), n);
    double new_arg = get_arg() * n;
    return Trigonometric(new_modulus, new_arg);
}

// to string

std::string Complex::to_string() const {
    // TODO
    return "";
}
