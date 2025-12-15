#include <gtest/gtest.h>
#include "math/complex.hpp"

using namespace iheay::math;

// Тестирование конструктора и статических методов
TEST(ComplexTests, Creation) {
    Complex z1 = Complex::Zero();
    EXPECT_DOUBLE_EQ(z1.get_real(), 0.0);
    EXPECT_DOUBLE_EQ(z1.get_imag(), 0.0);

    Complex z2 = Complex::Algebraic(3.0, 4.0);
    EXPECT_DOUBLE_EQ(z2.get_real(), 3.0);
    EXPECT_DOUBLE_EQ(z2.get_imag(), 4.0);

    Complex z3 = Complex::Trigonometric(5.0, M_PI / 2);
    EXPECT_NEAR(z3.get_real(), 0.0, 1e-9);
    EXPECT_NEAR(z3.get_imag(), 5.0, 1e-9);
}

// Тестирование арифметических операций
TEST(ComplexTests, Arithmetic) {
    Complex a = Complex::Algebraic(1.0, 2.0);
    Complex b = Complex::Algebraic(3.0, 4.0);

    Complex sum = a + b;
    EXPECT_DOUBLE_EQ(sum.get_real(), 4.0);
    EXPECT_DOUBLE_EQ(sum.get_imag(), 6.0);

    Complex diff = a - b;
    EXPECT_DOUBLE_EQ(diff.get_real(), -2.0);
    EXPECT_DOUBLE_EQ(diff.get_imag(), -2.0);

    Complex prod = a * b;
    EXPECT_DOUBLE_EQ(prod.get_real(), -5.0);
    EXPECT_DOUBLE_EQ(prod.get_imag(), 10.0);

    Complex quot = b / a;
    EXPECT_NEAR(quot.get_real(), 2.2, 1e-9);   // (3+4i)/(1+2i) = 2.2 -0.4i
    EXPECT_NEAR(quot.get_imag(), -0.4, 1e-9);
}

// Тестирование унарных операторов
TEST(ComplexTests, UnaryOperators) {
    Complex z = Complex::Algebraic(3.0, -4.0);

    Complex neg = -z;
    EXPECT_DOUBLE_EQ(neg.get_real(), -3.0);
    EXPECT_DOUBLE_EQ(neg.get_imag(), 4.0);

    Complex conj = ~z;
    EXPECT_DOUBLE_EQ(conj.get_real(), 3.0);
    EXPECT_DOUBLE_EQ(conj.get_imag(), 4.0);
}

// Тестирование модуля и аргумента
TEST(ComplexTests, ModulusAndArgument) {
    Complex z = Complex::Algebraic(3.0, 4.0);
    EXPECT_DOUBLE_EQ(z.get_modulus(), 5.0);
    EXPECT_DOUBLE_EQ(z.get_arg(), std::atan2(4.0, 3.0));
}

// Тестирование to_string (пример)
TEST(ComplexTests, ToString) {
    Complex z = Complex::Algebraic(3.0, -4.0);
    std::string s = z.to_string();
    EXPECT_FALSE(s.empty());
}
