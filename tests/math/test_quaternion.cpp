#include <gtest/gtest.h>
#include <cmath>
#include <vector>

#include "math/vec3.hpp"
#include "math/complex.hpp"
#include "math/quaternion.hpp"

using namespace iheay::math;

constexpr double EPS = 1e-5;

static void ExpectComplexNear(const Complex& actual, double real, double imag, double eps = EPS) {
    EXPECT_NEAR(actual.get_real(), real, eps);
    EXPECT_NEAR(actual.get_imag(), imag, eps);
}

static void ExpectQuaternionNear(const Quaternion& q, const Complex& z0, const Complex& z1, double eps = EPS) {
    ExpectComplexNear(q.get_z0(), z0.get_real(), z0.get_imag(), eps);
    ExpectComplexNear(q.get_z1(), z1.get_real(), z1.get_imag(), eps);
}

static void ExpectQuaternionNear(const Quaternion& q, double a, double b, double c, double d, double eps = EPS) {
    ExpectComplexNear(q.get_z0(), a, b, eps);
    ExpectComplexNear(q.get_z1(), c, d, eps);
}

TEST(QuaternionTest, ConstructorsAndModulus) {
    Complex a = Complex::Algebraic(1.0, 2.0);
    Complex b = Complex::Algebraic(3.0, -4.0);

    Quaternion q1(a, b);

    ExpectQuaternionNear(q1, 1, 2, 3, -4);

    double expected_modulus = std::hypot(a.get_modulus(), b.get_modulus());
    
    EXPECT_NEAR(q1.get_modulus(), expected_modulus, EPS);
}

TEST(QuaternionTest, ArithmeticOperations) {
    Quaternion q1(Complex::Algebraic(1, 2), Complex::Algebraic(3, -1));
    Quaternion q2(Complex::Algebraic(-2, 4), Complex::Algebraic(0, 5));

    ExpectQuaternionNear(q1 + q2, -1, 6, 3, 4);

    ExpectQuaternionNear(q1 - q2, 3, -2, 3, -6);

    ExpectQuaternionNear(q1 * q2, -5, 15, -20, -5);
}

TEST(QuaternionTest, UnaryOperations) {
    Quaternion q(Complex::Algebraic(1, -2), Complex::Algebraic(3, 4));

    ExpectQuaternionNear(-q, -1, 2, -3, -4);

    ExpectQuaternionNear(~q, 1, 2, -3, -4);
}

TEST(QuaternionTest, Inverse) {
    Quaternion q(Complex::Algebraic(1, 2), Complex::Algebraic(3, 4));
    Quaternion inv = q.inverse();

    ExpectQuaternionNear(inv, 0.033333, -0.066667, -0.100000, -0.133333);

    ExpectQuaternionNear(q * inv, 1, 0, 0, 0);
}

TEST(QuaternionTest, Power) {
    Quaternion q(Complex::Algebraic(1, 1), Complex::Algebraic(0, 1));
    Quaternion q2 = q.pow(2);

    Quaternion q_pow_manual = q * q;
    ExpectComplexNear(q2.get_z0(), q_pow_manual.get_z0().get_real(), q_pow_manual.get_z0().get_imag());
    ExpectComplexNear(q2.get_z1(), q_pow_manual.get_z1().get_real(), q_pow_manual.get_z1().get_imag());
}

TEST(QuaternionTest, ComplexCombination) {
    Quaternion q1(Complex::Algebraic(2, -1), Complex::Algebraic(1, 3));
    Quaternion q2(Complex::Algebraic(-1, 4), Complex::Algebraic(2, -2));


    ExpectQuaternionNear(q1 + q2, 1, 3, 3, 1);

    ExpectQuaternionNear(q1 - q2, 3, -5, -1, 5);

    ExpectQuaternionNear(q1 * q2, 6, 1, 13, -13);
}

static void ExpectVec3Near(const Vec3& v, double x, double y, double z, double eps = EPS) {
    EXPECT_NEAR(v.get_x(), x, eps);
    EXPECT_NEAR(v.get_y(), y, eps);
    EXPECT_NEAR(v.get_z(), z, eps);
}

TEST(QuaternionRotationTest, RotateAroundX) {
    Vec3 point(0, 1, 0);
    double angle = M_PI / 2; // 90 degrees
    Vec3 rotated = Quaternion::rotate_x(point, angle);

    ExpectVec3Near(rotated, 0, 0, 1); // Y -> Z
}

TEST(QuaternionRotationTest, RotateAroundY) {
    Vec3 point(1, 0, 0);
    double angle = M_PI / 2; // 90 degrees
    Vec3 rotated = Quaternion::rotate_y(point, angle);

    ExpectVec3Near(rotated, 0, 0, -1); // X -> -Z
}

TEST(QuaternionRotationTest, RotateAroundZ) {
    Vec3 point(1, 0, 0);
    double angle = M_PI / 2; // 90 degrees
    Vec3 rotated = Quaternion::rotate_z(point, angle);

    ExpectVec3Near(rotated, 0, 1, 0); // X -> Y
}

TEST(QuaternionRotationTest, RotateArbitraryAxis) {
    Vec3 point(1, 0, 0);
    Vec3 axis(0, 1, 0); // rotate around Y
    double angle = M_PI; // 180 degrees
    Vec3 rotated = Quaternion::rotate_point(point, axis, angle);

    ExpectVec3Near(rotated, -1, 0, 0); // X -> -X
}

TEST(QuaternionRotationTest, RotateZeroVector) {
    Vec3 point(0, 0, 0);
    Vec3 axis(1, 0, 0);
    double angle = M_PI / 3;
    Vec3 rotated = Quaternion::rotate_point(point, axis, angle);

    ExpectVec3Near(rotated, 0, 0, 0); // zero vector stays zero
}

TEST(QuaternionRotationTest, RotateIdentity) {
    Vec3 point(1, 2, 3);
    Vec3 rotated = Quaternion::rotate_point(point, {1, 0, 0}, 0); // 0 degrees
    ExpectVec3Near(rotated, 1, 2, 3); // point unchanged
}

TEST(QuaternionRotationTest, RotateBackAndForth) {
    Vec3 point(1, 1, 1);
    Vec3 axis(1, 0, 0);
    double angle = M_PI / 3;

    Vec3 rotated = Quaternion::rotate_point(point, axis, angle);
    Vec3 rotated_back = Quaternion::rotate_point(rotated, axis, -angle);

    ExpectVec3Near(rotated_back, 1, 1, 1); // rotation back restores point
}

