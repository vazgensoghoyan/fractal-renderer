#pragma once // math/complex.hpp

#include "math/constants.hpp"
#include <vector>
#include <cmath>
#include <stdexcept>
#include <ostream>
#include <numbers>
#include <format>

namespace iheay::math {

class Complex {
public:
    // constructors and fabrics

    constexpr Complex() noexcept : m_real(0.0), m_imag(0.0) {}

    constexpr Complex(double real) noexcept : m_real(real), m_imag(0.0) {}

    constexpr Complex(double real, double imag) noexcept : m_real(real), m_imag(imag) {}

    [[nodiscard]] static constexpr Complex Zero() noexcept { return Complex(0.0, 0.0); }
    
    [[nodiscard]] static constexpr Complex Algebraic(double real, double imag) noexcept {
        return Complex(real, imag);
    }
    
    [[nodiscard]] static Complex Trigonometric(double modulus, double arg) {
        if (modulus < 0)
            throw std::runtime_error("Modulus must be non-negative");
        arg = normalize_arg(arg);
        return Complex(modulus * std::cos(arg), modulus * std::sin(arg));
    }

    // properties

    [[nodiscard]] constexpr double real() const noexcept { return m_real; }
    [[nodiscard]] constexpr double imag() const noexcept { return m_imag; }

    [[nodiscard]] double modulus_squared() const noexcept { return m_real * m_real + m_imag * m_imag; }
    [[nodiscard]] double modulus() const noexcept { return std::hypot(m_real, m_imag); }
    [[nodiscard]] double arg() const noexcept { return normalize_arg(std::atan2(m_imag, m_real)); }

    // arithmetic operators

    [[nodiscard]] constexpr Complex operator-() const noexcept { return Complex(-m_real, -m_imag); }
    [[nodiscard]] constexpr Complex operator~() const noexcept { return Complex(m_real, -m_imag); } // conjugate

    [[nodiscard]] constexpr Complex operator+(const Complex& o) const noexcept { return Complex(m_real + o.m_real, m_imag + o.m_imag); }
    [[nodiscard]] constexpr Complex operator-(const Complex& o) const noexcept { return *this + (-o); }
    [[nodiscard]] constexpr Complex operator*(const Complex& o) const noexcept { return Complex(m_real * o.m_real - m_imag * o.m_imag, m_real * o.m_imag + m_imag * o.m_real); }
    [[nodiscard]] Complex operator/(const Complex& o) const {
        double denom = o.m_real * o.m_real + o.m_imag * o.m_imag;
        if (denom <= EPS)
            throw std::runtime_error("Division by complex zero");
        return Complex((m_real * o.m_real + m_imag * o.m_imag) / denom, (m_imag * o.m_real - m_real * o.m_imag) / denom);
    }

    [[nodiscard]] constexpr Complex operator*(double scalar) const noexcept {
        return Complex(m_real * scalar, m_imag * scalar);
    }

    [[nodiscard]] friend constexpr Complex operator*(double scalar, const Complex& c) noexcept {
        return c * scalar;
    }

    [[nodiscard]] Complex operator/(double scalar) const {
        if (std::abs(scalar) <= EPS) throw std::runtime_error("Division by zero");
        return Complex(m_real / scalar, m_imag / scalar);
    }

    Complex& operator+=(const Complex& o) noexcept { *this = *this + o; return *this; }
    Complex& operator-=(const Complex& o) noexcept { *this = *this - o; return *this; }
    Complex& operator*=(const Complex& o) noexcept { *this = *this * o; return *this; }
    Complex& operator/=(const Complex& o) { *this = *this / o; return *this; }

    // comparison

    [[nodiscard]] bool operator==(const Complex& o) const noexcept {
        return std::abs(m_real - o.m_real) <= EPS && std::abs(m_imag - o.m_imag) <= EPS;
    }

    [[nodiscard]] bool operator!=(const Complex& o) const noexcept {
        return !(*this == o);
    }

    // pow and roots

    [[nodiscard]] Complex pow(int n) const { 
        return Trigonometric(std::pow(modulus(), n), arg() * n); 
    }

    [[nodiscard]] std::vector<Complex> take_roots(int n) const {
        if (n <= 0)
            throw std::runtime_error("n must be positive");

        std::vector<Complex> roots;
        roots.reserve(n);

        double root_modulus = std::pow(modulus(), 1.0 / n);
        double theta = arg() / n;

        for (int k = 0; k < n; ++k) {
            roots.push_back(Trigonometric(root_modulus, theta + 2 * std::numbers::pi * k / n));
        }

        return roots;
    }

    // printing

    friend inline std::ostream& operator<<(std::ostream& os, const Complex& c) {
        return os << std::format("{} + i*{}", c.real(), c.imag());
    }

private:
    static double normalize_arg(double arg) noexcept {
        constexpr double TWO_PI = 2.0 * std::numbers::pi;
        double res = std::fmod(arg, TWO_PI);
        if (res < 0) res += TWO_PI;
        return res;
    }

private:
    double m_real;
    double m_imag;
};

} // namespace iheay::math
