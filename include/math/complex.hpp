#pragma once // math/complex.hpp

#include <vector>
#include <string>
#include <format>
#include <cmath>
#include <stdexcept>

namespace iheay::math {

class Complex {
public:
    Complex();
    Complex(double real);
    
    static Complex Zero();
    static Complex Algebraic(double real, double imag);
    static Complex Trigonometric(double modulus, double arg);

    constexpr double real() const { return m_real; }
    constexpr double imag() const { return m_imag; }
    
    double modulus() const;
    double arg() const; // in [0, 2pi)

    Complex operator+(const Complex& o) const;
    Complex operator-(const Complex& o) const;
    Complex operator*(const Complex& o) const;
    Complex operator/(const Complex& o) const;

    Complex operator-() const;
    Complex operator~() const; // conjugate 
    
    Complex operator*(double scalar) const;
    Complex operator/(double scalar) const;
    
    Complex& operator+=(const Complex& o);
    Complex& operator-=(const Complex& o);
    Complex& operator*=(const Complex& o);
    Complex& operator/=(const Complex& o);

    std::vector<Complex> take_roots(int n) const;
    Complex pow(int n) const;

private:
    Complex(double real, double imag);

private:
    double m_real;
    double m_imag;
};

Complex operator*(double scalar, Complex complex);

std::ostream& operator<<(std::ostream& os, const Complex& c);

} // namespace iheay::math
