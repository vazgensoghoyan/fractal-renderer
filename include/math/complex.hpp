#pragma once // math/complex.hpp

#include <vector>
#include <cmath>
#include <stdexcept>
#include <ostream>
#include <format>
#include <compare>

namespace iheay::math {

class Complex {
public:
    constexpr Complex() noexcept : m_real(0.0), m_imag(0.0) {}

    constexpr Complex(double real) noexcept : m_real(real), m_imag(0.0) {}

    constexpr Complex(double real, double imag) noexcept : m_real(real), m_imag(imag) {}

    [[nodiscard]] static constexpr Complex Zero() noexcept { return Complex(0.0, 0.0); }
    
    [[nodiscard]] static constexpr Complex Algebraic(double real, double imag) noexcept {
        return Complex(real, imag);
    }
    
    [[nodiscard]] static Complex Trigonometric(double modulus, double arg) {
        if (modulus < 0)
            throw std::domain_error("Modulus must be non-negative");
        arg = normalize_arg(arg);
        return Complex(modulus * std::cos(arg), modulus * std::sin(arg));
    }

    [[nodiscard]] constexpr double real() const noexcept { return m_real; }
    [[nodiscard]] constexpr double imag() const noexcept { return m_imag; }

    [[nodiscard]] double modulus() const noexcept { return std::hypot(m_real, m_imag); }
    [[nodiscard]] double arg() const noexcept { return normalize_arg(std::atan2(m_imag, m_real)); }

    [[nodiscard]] constexpr Complex operator-() const noexcept { return Complex(-m_real, -m_imag); }
    [[nodiscard]] constexpr Complex operator~() const noexcept { return Complex(m_real, -m_imag); } // conjugate

    [[nodiscard]] constexpr Complex operator+(const Complex& o) const noexcept { return Complex(m_real + o.m_real, m_imag + o.m_imag); }
    [[nodiscard]] constexpr Complex operator-(const Complex& o) const noexcept { return *this + (-o); }
    [[nodiscard]] constexpr Complex operator*(const Complex& o) const noexcept { return Complex(m_real * o.m_real - m_imag * o.m_imag, m_real * o.m_imag + m_imag * o.m_real); }
    [[nodiscard]] Complex operator/(const Complex& o) const {
        double denom = o.m_real * o.m_real + o.m_imag * o.m_imag;
        if (denom == 0)
            throw std::runtime_error("Division by complex zero");
        return Complex((m_real * o.m_real + m_imag * o.m_imag) / denom, (m_imag * o.m_real - m_real * o.m_imag) / denom);
    }

    [[nodiscard]] constexpr Complex operator*(double scalar) const noexcept { return Complex(m_real * scalar, m_imag * scalar); }
    [[nodiscard]] Complex operator/(double scalar) const {
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return Complex(m_real / scalar, m_imag / scalar);
    }

    friend constexpr Complex operator*(double scalar, const Complex& c) noexcept { return c * scalar; }

    Complex& operator+=(const Complex& o) noexcept { *this = *this + o; return *this; }
    Complex& operator-=(const Complex& o) noexcept { *this = *this - o; return *this; }
    Complex& operator*=(const Complex& o) noexcept { *this = *this * o; return *this; }
    Complex& operator/=(const Complex& o) { *this = *this / o; return *this; }

    [[nodiscard]] Complex pow(int n) const { 
        return Trigonometric(std::pow(modulus(), n), arg() * n); 
    }

    [[nodiscard]] std::vector<Complex> take_roots(int n) const {
        if (n <= 0)
            throw std::invalid_argument("n must be positive");

        std::vector<Complex> roots;
        roots.reserve(n);

        double root_modulus = std::pow(modulus(), 1.0 / n);
        double theta = arg() / n;

        for (int k = 0; k < n; ++k) {
            roots.push_back(Trigonometric(root_modulus, theta + 2 * M_PI * k / n));
        }

        return roots;
    }

    [[nodiscard]] constexpr auto operator<=>(const Complex&) const = default;
    
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        return os << std::format("{} + i*{}", c.real(), c.imag());
    }

private:
    static double normalize_arg(double arg) noexcept {
        constexpr double TWO_PI = 2.0 * M_PI;
        double res = std::fmod(arg, TWO_PI);
        if (res < 0) res += TWO_PI;
        return res;
    }

private:
    double m_real;
    double m_imag;
};

} // namespace iheay::math
