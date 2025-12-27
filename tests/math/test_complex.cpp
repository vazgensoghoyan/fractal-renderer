#include <gtest/gtest.h>
#include <cmath>
#include <vector>

#include "math/complex.hpp"

using namespace iheay::math;

constexpr double EPS = 1e-5;

static void ExpectComplexNear(const Complex& actual, double real, double imag, double eps = EPS) {
    EXPECT_NEAR(actual.real(), real, eps);
    EXPECT_NEAR(actual.imag(), imag, eps);
}

static void ExpectComplexEqual(const Complex& a, const Complex& b, double eps = EPS) {
    EXPECT_NEAR(a.real(), b.real(), eps);
    EXPECT_NEAR(a.imag(), b.imag(), eps);
}

TEST(ComplexTest, FactoryMethods) {
    ExpectComplexNear(Complex::Zero(), 0.0, 0.0);

    auto c1 = Complex::Algebraic(3.0, 4.0);
    ExpectComplexNear(c1, 3.0, 4.0);
    EXPECT_NEAR(c1.modulus(), 5.0, EPS);

    double r = 2.0, theta = M_PI / 4;
    auto c2 = Complex::Trigonometric(r, theta);
    EXPECT_NEAR(c2.modulus(), r, EPS);
    EXPECT_NEAR(c2.arg(), theta, EPS);
}

TEST(ComplexTest, ArithmeticOperations) {
    auto a = Complex::Algebraic(1.0, 2.0);
    auto b = Complex::Algebraic(3.0, -4.0);

    ExpectComplexNear(a + b, 4.0, -2.0);
    ExpectComplexNear(a - b, -2.0, 6.0);
    ExpectComplexNear(a * b, 11.0, 2.0);
    ExpectComplexNear(a / b, -0.2, 0.4);
}

TEST(ComplexTest, UnaryOperations) {
    auto c = Complex::Algebraic(5.0, -7.0);
    ExpectComplexNear(-c, -5.0, 7.0);
    ExpectComplexNear(~c, 5.0, 7.0);
}

TEST(ComplexTest, PowerAndRoots) {
    auto c = Complex::Algebraic(1.0, 1.0);
    ExpectComplexNear(c.pow(2), 0.0, 2.0);

    auto roots = c.get_roots(2);
    EXPECT_EQ(roots.size(), 2);
    for (auto& root : roots) {
        auto sq = root.pow(2);
        ExpectComplexNear(sq, c.real(), c.imag());
    }
}

TEST(ComplexTest, ArgAndModulus) {
    auto c1 = Complex::Algebraic(0.0, 1.0);
    EXPECT_NEAR(c1.modulus(), 1.0, EPS);
    EXPECT_NEAR(c1.arg(), M_PI / 2, EPS);

    auto c2 = Complex::Algebraic(-1.0, 0.0);
    EXPECT_NEAR(c2.arg(), M_PI, EPS);
}

TEST(ComplexTest, ArgumentSpecialCases) {
    EXPECT_NEAR(Complex::Algebraic(0, 11).arg(), M_PI / 2, EPS);
    EXPECT_NEAR(Complex::Algebraic(0, -100).arg(), 3 * M_PI / 2, EPS);
    EXPECT_NEAR(Complex::Algebraic(117, -1).arg(), std::atan(-1.0 / 117) + 2 * M_PI, EPS);
    EXPECT_NEAR(Complex::Algebraic(-708, 19).arg(), M_PI + std::atan(-19.0 / 708), EPS);
    EXPECT_NEAR(Complex::Algebraic(-78, -13).arg(), M_PI + std::atan(13.0 / 78), EPS);
}

TEST(ComplexTest, BasicArithmetic) {
    ExpectComplexEqual(Complex::Algebraic(1,3) + Complex::Algebraic(4,-5), Complex::Algebraic(5,-2));
    ExpectComplexEqual(Complex::Algebraic(-2,1) - Complex::Algebraic(std::sqrt(3),5), Complex::Algebraic(-2-std::sqrt(3),-4));
    ExpectComplexEqual(Complex::Algebraic(-2,1) - Complex::Algebraic(std::sqrt(3),5), Complex::Algebraic(-2-std::sqrt(3),-4));
    ExpectComplexEqual(Complex::Algebraic(1,-1) * Complex::Algebraic(3,6), Complex::Algebraic(9,3));
    ExpectComplexEqual(Complex::Algebraic(13,1) / Complex::Algebraic(7,-6), Complex::Algebraic(1,1));
}

TEST(ComplexTest, DivisionAndExponentiation) {
    ExpectComplexEqual(
        Complex::Algebraic(1,0) / Complex::Algebraic(std::sqrt(3),1),
        Complex::Algebraic(std::sqrt(3)/4, -0.25)
    );

    auto n = Complex::Algebraic(2, -std::sqrt(3));
    auto expected = Complex::Algebraic(11041, -7316 * std::sqrt(3));
    ExpectComplexEqual(n.pow(10), expected);
}

TEST(ComplexTest, RootExtraction) {
    auto n = Complex::Algebraic(-0.5, std::sqrt(3)/2);
    auto roots = n.get_roots(3);

    for (auto& r : roots) {
        auto r_cubed = r.pow(3);
        EXPECT_NEAR(r_cubed.real(), n.real(), EPS);
        EXPECT_NEAR(r_cubed.imag(), n.imag(), EPS);
    }
}
