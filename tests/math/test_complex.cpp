#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include <numbers>

#include "math/complex.hpp"

using namespace iheay::math;

TEST(ComplexTest, FactoryMethods) {
    EXPECT_TRUE(Complex::Zero() == Complex(0.0, 0.0));

    auto c1 = Complex(3.0, 4.0);
    EXPECT_TRUE(c1 == Complex(3.0, 4.0));
    EXPECT_NEAR(c1.modulus(), 5.0, 1e-12);

    double r = 2.0, theta = std::numbers::pi / 4;
    auto c2 = Complex::Trigonometric(r, theta);
    EXPECT_NEAR(c2.modulus(), r, 1e-12);
    EXPECT_NEAR(c2.arg(), theta, 1e-12);
}

TEST(ComplexTest, ArithmeticOperations) {
    auto a = Complex(1.0, 2.0);
    auto b = Complex(3.0, -4.0);

    EXPECT_TRUE(a + b == Complex(4.0, -2.0));
    EXPECT_TRUE(a - b == Complex(-2.0, 6.0));
    EXPECT_TRUE(a * b == Complex(11.0, 2.0));
    EXPECT_TRUE(a / b == Complex(-0.2, 0.4));
}

TEST(ComplexTest, UnaryOperations) {
    auto c = Complex(5.0, -7.0);
    EXPECT_TRUE(-c == Complex(-5.0, 7.0));
    EXPECT_TRUE(~c == Complex(5.0, 7.0));
}

TEST(ComplexTest, PowerAndRoots) {
    auto c = Complex(1.0, 1.0);
    EXPECT_TRUE(c.pow(2) == Complex(0.0, 2.0));

    auto roots = c.take_roots(2);
    EXPECT_EQ(roots.size(), 2);
    for (auto& root : roots) {
        auto sq = root.pow(2);
        EXPECT_TRUE(sq == c);
    }
}

TEST(ComplexTest, ArgAndModulus) {
    auto c1 = Complex(0.0, 1.0);
    EXPECT_NEAR(c1.modulus(), 1.0, 1e-12);
    EXPECT_NEAR(c1.arg(), std::numbers::pi / 2, 1e-12);

    auto c2 = Complex(-1.0, 0.0);
    EXPECT_NEAR(c2.arg(), std::numbers::pi, 1e-12);
}

TEST(ComplexTest, ArgumentSpecialCases) {
    EXPECT_NEAR(Complex(0, 11).arg(), std::numbers::pi / 2, 1e-12);
    EXPECT_NEAR(Complex(0, -100).arg(), 3 * std::numbers::pi / 2, 1e-12);
    EXPECT_NEAR(Complex(117, -1).arg(), std::atan(-1.0 / 117) + 2 * std::numbers::pi, 1e-12);
    EXPECT_NEAR(Complex(-708, 19).arg(), std::numbers::pi + std::atan(-19.0 / 708), 1e-12);
    EXPECT_NEAR(Complex(-78, -13).arg(), std::numbers::pi + std::atan(13.0 / 78), 1e-12);
}

TEST(ComplexTest, BasicArithmetic) {
    EXPECT_TRUE(Complex(1,3) + Complex(4,-5) == Complex(5,-2));
    EXPECT_TRUE(Complex(-2,1) - Complex(std::sqrt(3),5) == Complex(-2-std::sqrt(3),-4));
    EXPECT_TRUE(Complex(1,-1) * Complex(3,6) == Complex(9,3));
    EXPECT_TRUE(Complex(13,1) / Complex(7,-6) == Complex(1,1));
}

TEST(ComplexTest, DivisionAndExponentiation) {
    EXPECT_TRUE(Complex(1,0) / Complex(std::sqrt(3),1) == Complex(std::sqrt(3)/4, -0.25));

    auto n = Complex(2, -std::sqrt(3));
    auto expected = n; // тут лучше проверять не конкретные числа для pow(10), а через циклическое возведение
    //EXPECT_TRUE(n.pow(10).modulus_squared() > 0); // минимальная проверка, что возведение работает без NaN
}

TEST(ComplexTest, RootExtraction) {
    auto n = Complex(-0.5, std::sqrt(3)/2);
    auto roots = n.take_roots(3);

    for (auto& r : roots) {
        EXPECT_TRUE(r.pow(3) == n);
    }
}

TEST(ComplexTest, DivisionByZeroThrows) {
    Complex a(1.0, 2.0);
    Complex zero(0.0, 0.0);

    EXPECT_THROW((void)(a / zero), std::runtime_error);
    EXPECT_THROW((void)(a / 0.0), std::runtime_error);
}

TEST(ComplexTest, TrigonometricNegativeModulusThrows) {
    EXPECT_THROW((void)Complex::Trigonometric(-1.0, 0.0), std::runtime_error);
}

TEST(ComplexTest, TakeRootsInvalidNThrows) {
    Complex c(1.0, 1.0);
    EXPECT_THROW((void)c.take_roots(0), std::runtime_error);
    EXPECT_THROW((void)c.take_roots(-5), std::runtime_error);
}

TEST(ComplexTest, PowZeroAndNegative) {
    Complex c(1.0, 1.0);

    // pow 0 должно дать 1+0i
    Complex p0 = c.pow(0);
    EXPECT_NEAR(p0.real(), 1.0, EPS);
    EXPECT_NEAR(p0.imag(), 0.0, EPS);

    // pow отрицательной степени
    Complex p_neg = c.pow(-1);
    Complex expected = Complex(0.5, -0.5);
    EXPECT_NEAR(p_neg.real(), expected.real(), EPS);
    EXPECT_NEAR(p_neg.imag(), expected.imag(), EPS);
}

TEST(ComplexTest, ConjugateAndNegation) {
    Complex c(3.0, -4.0);
    Complex conj = ~c;
    EXPECT_TRUE(conj == Complex(3.0, 4.0));

    Complex neg = -c;
    EXPECT_TRUE(neg == Complex(-3.0, 4.0));
}

TEST(ComplexTest, ScalarMultiplicationAndDivision) {
    Complex c(2.0, -3.0);

    EXPECT_TRUE(c * 2.0 == Complex(4.0, -6.0));
    EXPECT_TRUE(2.0 * c == Complex(4.0, -6.0));
    EXPECT_TRUE(c / 2.0 == Complex(1.0, -1.5));

    EXPECT_THROW((void)(c / 0.0), std::runtime_error);
}

TEST(ComplexTest, CompoundAssignmentOperators) {
    Complex c(1.0, 2.0);
    Complex d(3.0, 4.0);

    Complex temp = c;
    temp += d;
    EXPECT_TRUE(temp == c + d);

    temp = c;
    temp -= d;
    EXPECT_TRUE(temp == c - d);

    temp = c;
    temp *= d;
    EXPECT_TRUE(temp == c * d);

    temp = c;
    temp /= d;
    EXPECT_TRUE(temp == c / d);
}

TEST(ComplexTest, ModulusSquaredAndArg) {
    Complex c(3.0, 4.0);
    EXPECT_DOUBLE_EQ(c.modulus_squared(), 25.0);
    EXPECT_DOUBLE_EQ(c.modulus(), 5.0);
    EXPECT_NEAR(c.arg(), std::atan2(4.0, 3.0), EPS);
}

TEST(ComplexTest, NormalizeArgRange) {
    Complex c(-1.0, -1.0);
    double arg = c.arg();
    EXPECT_GE(arg, 0.0);
    EXPECT_LT(arg, 2 * std::numbers::pi);
}
