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

        [[nodiscard]] double get_real() const;
        [[nodiscard]] double get_imag() const;
        [[nodiscard]] double get_modulus() const;
        [[nodiscard]] double get_arg() const; // in [0, 2pi)

        Complex operator+(const Complex& other) const;
        Complex operator-(const Complex& other) const;
        Complex operator*(const Complex& other) const;
        Complex operator/(const Complex& other) const;

        Complex operator-() const;
        Complex operator~() const; // conjugate 
        
        Complex operator/(double scalar) const;

        std::vector<Complex> get_roots(int n) const;
        Complex pow(int n) const;

        std::string to_string() const;

    private:
        Complex(double real, double imag);

    private:
        double m_real;
        double m_imag;

    };
    
}
