#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <vector>
#include <string>
#include <format>
#include <cmath>
#include <stdexcept>

class Complex {

public:
    static Complex Algebraic(double real, double imag);
    static Complex Trigonometric(double modulus, double arg);

    double get_modulus() const;
    double get_arg() const; // in [0, 2pi)

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;

    Complex operator-() const;
    Complex operator~() const; // conjugate 

    std::vector<Complex> get_roots(int n) const;
    Complex pow(int n) const;

    std::string to_string() const;

private:
    Complex(double real, double imag);

private:
    double real_;
    double imag_;

};

#endif // COMPLEX_HPP
