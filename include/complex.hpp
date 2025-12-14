#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

class Complex {

public:
    static Complex Algebraic(double real, double imag);
    static Complex Trigonometric(double modulus, double arg);

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
    Complex(double real, double imag, double modulus, double arg);

private:
    double real_;
    double imag_;
    double modulus_;
    double arg_; // it is in [0, 2pi)

};

#endif // COMPLEX_HPP
