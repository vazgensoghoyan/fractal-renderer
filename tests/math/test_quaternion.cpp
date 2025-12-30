#include <gtest/gtest.h>
#include <cmath>
#include "math/vec3.hpp"
#include "math/complex.hpp"
#include "math/quaternion.hpp"

using namespace iheay::math;

TEST(QuaternionTest, ConstructorsAndModulus) {
    Complex a(1.0, 2.0);
    Complex b(3.0, -4.0);

    Quaternion q1(a, b);
    EXPECT_TRUE(q1.z0() == a);
    EXPECT_TRUE(q1.z1() == b);

    double expected_modulus = std::hypot(a.modulus(), b.modulus());
    EXPECT_NEAR(q1.modulus(), expected_modulus, EPS);
}

TEST(QuaternionTest, ArithmeticOperations) {
    Quaternion q1(Complex(1, 2), Complex(3, -1));
    Quaternion q2(Complex(-2, 4), Complex(0, 5));

    EXPECT_TRUE((q1 + q2) == Quaternion(Complex(-1, 6), Complex(3, 4)));
    EXPECT_TRUE((q1 - q2) == Quaternion(Complex(3, -2), Complex(3, -6)));
    EXPECT_TRUE((q1 * q2) == Quaternion(Complex(-5, 15), Complex(-20, -5)));
}

TEST(QuaternionTest, UnaryOperations) {
    Quaternion q(Complex(1, -2), Complex(3, 4));
    EXPECT_TRUE(-q == Quaternion(Complex(-1, 2), Complex(-3, -4)));
    EXPECT_TRUE(~q == Quaternion(Complex(1, 2), Complex(-3, -4)));
}

TEST(QuaternionTest, Inverse) {
    Quaternion q(Complex(1, 2), Complex(3, 4));
    Quaternion inv = q.inverse();

    Quaternion prod = q * inv;
    EXPECT_TRUE(prod == Quaternion(1, 0, 0, 0));
}

TEST(QuaternionTest, Power) {
    Quaternion q(Complex(1, 1), Complex(0, 1));
    Quaternion q2 = q.pow(2);
    Quaternion q_manual = q * q;

    EXPECT_TRUE(q2.z0() == q_manual.z0());
    EXPECT_TRUE(q2.z1() == q_manual.z1());
}

TEST(QuaternionTest, ComplexCombination) {
    Quaternion q1(Complex(2, -1), Complex(1, 3));
    Quaternion q2(Complex(-1, 4), Complex(2, -2));

    EXPECT_TRUE((q1 + q2) == Quaternion(Complex(1, 3), Complex(3, 1)));
    EXPECT_TRUE((q1 - q2) == Quaternion(Complex(3, -5), Complex(-1, 5)));
    EXPECT_TRUE((q1 * q2) == Quaternion(Complex(6, 1), Complex(13, -13)));
}

TEST(QuaternionRotationTest, RotateAroundX) {
    Vec3 point(0, 1, 0);
    Vec3 rotated = Quaternion::rotate_x(point, M_PI / 2);
    EXPECT_TRUE(rotated == Vec3(0, 0, 1));
}

TEST(QuaternionRotationTest, RotateAroundY) {
    Vec3 point(1, 0, 0);
    Vec3 rotated = Quaternion::rotate_y(point, M_PI / 2);
    EXPECT_TRUE(rotated == Vec3(0, 0, -1));
}

TEST(QuaternionRotationTest, RotateAroundZ) {
    Vec3 point(1, 0, 0);
    Vec3 rotated = Quaternion::rotate_z(point, M_PI / 2);
    EXPECT_TRUE(rotated == Vec3(0, 1, 0));
}

TEST(QuaternionRotationTest, RotateArbitraryAxis) {
    Vec3 point(1, 0, 0);
    Vec3 axis(0, 1, 0);
    Vec3 rotated = Quaternion::rotate_point(point, axis, M_PI);
    EXPECT_TRUE(rotated == Vec3(-1, 0, 0));
}

TEST(QuaternionRotationTest, RotateZeroVector) {
    Vec3 point(0, 0, 0);
    Vec3 axis(1, 0, 0);
    Vec3 rotated = Quaternion::rotate_point(point, axis, M_PI / 3);
    EXPECT_TRUE(rotated == Vec3(0, 0, 0));
}

TEST(QuaternionRotationTest, RotateIdentity) {
    Vec3 point(1, 2, 3);
    Vec3 rotated = Quaternion::rotate_point(point, {1, 0, 0}, 0);
    EXPECT_TRUE(rotated == Vec3(1, 2, 3));
}

TEST(QuaternionRotationTest, RotateBackAndForth) {
    Vec3 point(1, 1, 1);
    Vec3 axis(1, 0, 0);
    double angle = M_PI / 3;

    Vec3 rotated = Quaternion::rotate_point(point, axis, angle);
    Vec3 rotated_back = Quaternion::rotate_point(rotated, axis, -angle);

    EXPECT_TRUE(rotated_back == Vec3(1, 1, 1));
}
