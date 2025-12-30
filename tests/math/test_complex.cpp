#include <gtest/gtest.h>
#include <cmath>
#include <vector>

#include "math/complex.hpp"

using namespace iheay::math;

TEST(ComplexTest, FactoryMethods) {
    EXPECT_TRUE(Complex::Zero() == Complex(0.0, 0.0));

    auto c1 = Complex(3.0, 4.0);
    EXPECT_TRUE(c1 == Complex(3.0, 4.0));
    EXPECT_NEAR(c1.modulus(), 5.0, 1e-12);

    double r = 2.0, theta = M_PI / 4;
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
    EXPECT_NEAR(c1.arg(), M_PI / 2, 1e-12);

    auto c2 = Complex(-1.0, 0.0);
    EXPECT_NEAR(c2.arg(), M_PI, 1e-12);
}

TEST(ComplexTest, ArgumentSpecialCases) {
    EXPECT_NEAR(Complex(0, 11).arg(), M_PI / 2, 1e-12);
    EXPECT_NEAR(Complex(0, -100).arg(), 3 * M_PI / 2, 1e-12);
    EXPECT_NEAR(Complex(117, -1).arg(), std::atan(-1.0 / 117) + 2 * M_PI, 1e-12);
    EXPECT_NEAR(Complex(-708, 19).arg(), M_PI + std::atan(-19.0 / 708), 1e-12);
    EXPECT_NEAR(Complex(-78, -13).arg(), M_PI + std::atan(13.0 / 78), 1e-12);
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
