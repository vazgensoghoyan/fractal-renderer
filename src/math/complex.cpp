#include "math/complex.hpp"

using namespace iheay::math;

// utils

static const double PI = acos(-1.0);

double normalize_arg(double arg) { // to [0, 2pi)
    double res = std::fmod(arg, 2 * PI);

    if (res < 0) res += 2 * PI;

    return res;
}

// empty public constructor

Complex::Complex() : m_real(0), m_imag(0) { }

// private constructor

Complex::Complex(double real, double imag) : m_real(real), m_imag(imag) { }

// little public fabric

Complex Complex::Zero() {
    return Complex();
}

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

    return Algebraic(real, imag);
}

// properties

double Complex::real() const {
    return m_real;
}

double Complex::imag() const {
    return m_imag;
}

double Complex::modulus() const {
    return std::hypot(m_real, m_imag);
}

double Complex::arg() const {
    double arg = atan2(m_imag, m_real);
    return normalize_arg(arg);
}

// unary operators

Complex Complex::operator-() const {
    return Algebraic(-m_real, -m_imag);
}

Complex Complex::operator~() const { // conjugate
    return Algebraic(m_real, -m_imag);
}

// binary operators

Complex Complex::operator+(const Complex& other) const {
    double new_real = m_real + other.m_real;
    double new_imag = m_imag + other.m_imag;

    return Algebraic(new_real, new_imag);
}

Complex Complex::operator-(const Complex& other) const {
    return *this + (-other);
}

Complex Complex::operator*(const Complex& other) const {
    double new_real = m_real * other.m_real - m_imag * other.m_imag;
    double new_imag = m_real * other.m_imag + m_imag * other.m_real;

    return Algebraic(new_real, new_imag);
}

Complex Complex::operator/(const Complex& other) const {
    double denom = other.m_real * other.m_real + other.m_imag * other.m_imag;

    if (denom == 0)
        throw std::runtime_error("Cant divide by complex zero");

    double new_real = (m_real * other.m_real + m_imag * other.m_imag) / denom;
    double new_imag = (m_imag * other.m_real - m_real * other.m_imag) / denom;

    return Algebraic(new_real, new_imag);
}

// oper with double

Complex Complex::operator/(double scalar) const {
    return Algebraic(m_real / scalar, m_imag / scalar);
}

// methods

std::vector<Complex> Complex::take_roots(int n) const {
    std::vector<Complex> roots(n);
    
    double res_modulus = std::pow(modulus(), 1.0 / n);
    double arg_i = arg() / n;

    for (int i = 0; i < n; i++) {
        roots[i] = Trigonometric(res_modulus, arg_i);
        arg_i += 2 * PI / n;
    }

    return roots;
}

Complex Complex::pow(int n) const { // simple recursive binary pow
    double new_modulus = std::pow(modulus(), n);
    double new_arg = arg() * n;
    return Trigonometric(new_modulus, new_arg);
}

// to string

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    return os << std::format("{} + i * {}", c.real(), c.imag());
}
