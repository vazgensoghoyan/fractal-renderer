#include <gtest/gtest.h>
#include <cmath>
#include "math/vec3.hpp"

using namespace iheay::math;

TEST(Vec3Test, ConstructorsAndAccessors) {
    Vec3 v1;
    EXPECT_DOUBLE_EQ(v1.x(), 0.0);
    EXPECT_DOUBLE_EQ(v1.y(), 0.0);
    EXPECT_DOUBLE_EQ(v1.z(), 0.0);

    Vec3 v2(1.0, -2.0, 3.5);
    EXPECT_DOUBLE_EQ(v2.x(), 1.0);
    EXPECT_DOUBLE_EQ(v2.y(), -2.0);
    EXPECT_DOUBLE_EQ(v2.z(), 3.5);
}

TEST(Vec3Test, LengthAndLengthSquared) {
    Vec3 v(2.0, 3.0, 6.0);
    EXPECT_DOUBLE_EQ(v.length_squared(), 2*2 + 3*3 + 6*6);
    EXPECT_NEAR(v.length(), std::sqrt(49.0), EPS);
}

TEST(Vec3Test, UnaryOperators) {
    Vec3 v(1.0, -2.0, 3.0);
    Vec3 neg = -v;
    EXPECT_TRUE(neg == Vec3(-1.0, 2.0, -3.0));
}

TEST(Vec3Test, AdditionAndSubtraction) {
    Vec3 a(1, 2, 3);
    Vec3 b(4, -1, 0.5);

    EXPECT_TRUE(a + b == Vec3(5, 1, 3.5));
    EXPECT_TRUE(a - b == Vec3(-3, 3, 2.5));

    Vec3 c = a;
    c += b;
    EXPECT_TRUE(c == a + b);

    c = a;
    c -= b;
    EXPECT_TRUE(c == a - b);
}

TEST(Vec3Test, ScalarMultiplicationAndDivision) {
    Vec3 v(1.0, -2.0, 3.0);
    EXPECT_TRUE(v * 2.0 == Vec3(2.0, -4.0, 6.0));
    EXPECT_TRUE(3.0 * v == Vec3(3.0, -6.0, 9.0));

    Vec3 w = v;
    w *= 2.0;
    EXPECT_TRUE(w == Vec3(2.0, -4.0, 6.0));

    Vec3 div = v / 2.0;
    EXPECT_TRUE(div == Vec3(0.5, -1.0, 1.5));

    Vec3 x = v;
    x /= 2.0;
    EXPECT_TRUE(x == Vec3(0.5, -1.0, 1.5));
}

TEST(Vec3Test, DivisionByZeroThrows) {
    Vec3 v(1.0, 2.0, 3.0);
    EXPECT_THROW((void)(v / 0.0), std::runtime_error);
    EXPECT_THROW((void)(v /= 0.0), std::runtime_error);
}

TEST(Vec3Test, Normalization) {
    Vec3 v(3.0, 0.0, 4.0);
    Vec3 n = v.normalized();
    EXPECT_NEAR(n.length(), 1.0, EPS);
    EXPECT_TRUE(n == Vec3(0.6, 0.0, 0.8));

    Vec3 zero;
    EXPECT_THROW((void)zero.normalized(), std::runtime_error);
}

TEST(Vec3Test, DotAndCross) {
    Vec3 a(1, 2, 3);
    Vec3 b(4, 5, 6);

    double dot_val = Vec3::dot(a, b);
    EXPECT_DOUBLE_EQ(dot_val, 1*4 + 2*5 + 3*6);

    Vec3 cross_val = Vec3::cross(a, b);
    EXPECT_TRUE(cross_val == Vec3(-3, 6, -3));
}

TEST(Vec3Test, ComparisonOperators) {
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(1.0 + EPS/2, 2.0 - EPS/2, 3.0);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    Vec3 c(1.0, 2.0, 3.1);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(Vec3Test, StreamOutput) {
    Vec3 v(1.0, -2.0, 3.5);
    std::ostringstream os;
    os << v;
    EXPECT_EQ(os.str(), "(1, -2, 3.5)");
}
