#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <vector>
#include <string>

class Complex {

public:
    static Complex Algebraic(double real, double imag);
    static Complex Trigonometric(double modulus, double arg);

    Complex operator+(const Complex& other);
    Complex operator-(const Complex& other);
    Complex operator*(const Complex& other);
    Complex operator/(const Complex& other);

    Complex operator-();
    Complex operator~();

    std::vector<Complex> get_roots(int n);
    Complex pow(int n);

    std::string to_string();

private:
    Complex();

private:
    double real_;
    double imag_;
    double modulus_;
    double arg_;

};

#endif // COMPLEX_HPP
