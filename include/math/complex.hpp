#pragma once // complex.hpp

#include <vector>
#include <string>
#include <format>
#include <cmath>
#include <stdexcept>

namespace iheay::math {

class Complex {
public:
    Complex();
    
    static Complex Zero();
    static Complex Algebraic(double real, double imag);
    static Complex Trigonometric(double modulus, double arg);

    double real() const;
    double imag() const;
    double modulus() const;
    double arg() const; // in [0, 2pi)

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;

    Complex operator-() const;
    Complex operator~() const; // conjugate 
    
    Complex operator/(double scalar) const;

    std::vector<Complex> get_roots(int n) const;
    Complex pow(int n) const;

private:
    Complex(double real, double imag);

private:
    double m_real;
    double m_imag;
};

std::ostream& operator<<(std::ostream& os, const Complex& c);

} // namespace iheay::math
