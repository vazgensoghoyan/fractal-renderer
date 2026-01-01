#pragma once // math/quaternion.hpp

#include "math/complex.hpp"
#include "math/vec3.hpp"
#include "math/constants.hpp"
#include <cmath>
#include <ostream>
#include <format>
#include <stdexcept>

namespace iheay::math {

class Quaternion {
public:
    // constructors

    Quaternion(double a, double b, double c, double d)
        : m_z0(Complex::Algebraic(a, b))
        , m_z1(Complex::Algebraic(c, d)) {}

    Quaternion(Complex z0, Complex z1) noexcept : m_z0(z0), m_z1(z1) {}

    Quaternion(double a, const Vec3& vec)
        : m_z0(Complex::Algebraic(a, vec.x()))
        , m_z1(Complex::Algebraic(vec.y(), vec.z())) {}

    // properties

    [[nodiscard]] constexpr double a() const noexcept { return m_z0.real(); }
    [[nodiscard]] constexpr double b() const noexcept { return m_z0.imag(); }
    [[nodiscard]] constexpr double c() const noexcept { return m_z1.real(); }
    [[nodiscard]] constexpr double d() const noexcept { return m_z1.imag(); }

    [[nodiscard]] const Complex& z0() const noexcept { return m_z0; }
    [[nodiscard]] const Complex& z1() const noexcept { return m_z1; }

    [[nodiscard]] double modulus_squared() const noexcept { return a() * a() + b() * b() + c() * c() + d() * d(); }
    [[nodiscard]] double modulus() const noexcept { return std::sqrt(modulus_squared()); }

    // arithmetic operators

    [[nodiscard]] Quaternion operator-() const noexcept { return Quaternion(-m_z0, -m_z1); }
    [[nodiscard]] Quaternion operator~() const noexcept { return Quaternion(~m_z0, -m_z1); } // conjugate

    [[nodiscard]] Quaternion operator+(const Quaternion& o) const noexcept { return Quaternion(m_z0 + o.m_z0, m_z1 + o.m_z1); }
    [[nodiscard]] Quaternion operator-(const Quaternion& o) const noexcept { return *this + (-o); }
    [[nodiscard]] Quaternion operator*(const Quaternion& o) const {
        return Quaternion(m_z0 * o.m_z0 - m_z1 * (~o.m_z1), m_z0 * o.m_z1 + m_z1 * (~o.m_z0));
    }

    Quaternion& operator+=(const Quaternion& o) noexcept { *this = *this + o; return *this; }
    Quaternion& operator-=(const Quaternion& o) noexcept { *this = *this - o; return *this; }
    Quaternion& operator*=(const Quaternion& o) { *this = *this * o; return *this; }

    // comparison

    [[nodiscard]] bool operator==(const Quaternion& o) const noexcept {
        return m_z0 == o.m_z0 && m_z1 == o.m_z1;
    }

    [[nodiscard]] bool operator!=(const Quaternion& o) const noexcept {
        return !(*this == o);
    }

    // pow and inverse

    [[nodiscard]] Quaternion inverse() const {
        double ms = modulus_squared();
        if (ms <= EPS * EPS) throw std::runtime_error("Inverse of zero quaternion");
        return Quaternion(~m_z0 / ms, -m_z1 / ms);
    }

    [[nodiscard]] Quaternion pow(int n) const {
        Quaternion result(1,0,0,0);
        Quaternion base = *this;
        int exp = std::abs(n);
        while (exp > 0) {
            if (exp & 1) result *= base;
            base *= base;
            exp >>= 1;
        }
        if (n < 0) result = result.inverse();
        return result;
    }

    // 3d-rotations using quaternions

    static Vec3 rotate_point(const Vec3& point, const Vec3& axis, double phi) {
        if (axis.length_squared() <= EPS * EPS) 
            throw std::runtime_error("Rotation axis is zero");
        double half_phi = phi / 2;
        Vec3 s = axis.normalized() * sin(half_phi);
        Quaternion q(std::cos(half_phi), s);
        Quaternion v(0, point);
        Quaternion rotated = q * v * ~q; // == q * v * q.inverse()
        return Vec3(rotated.b(), rotated.c(), rotated.d());
    }

    static Vec3 rotate_x(const Vec3& point, double phi) { return rotate_point(point, {1,0,0}, phi); }
    static Vec3 rotate_y(const Vec3& point, double phi) { return rotate_point(point, {0,1,0}, phi); }
    static Vec3 rotate_z(const Vec3& point, double phi) { return rotate_point(point, {0,0,1}, phi); }

    // printing

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
        return os << std::format("{} + i*{} + j*{} + k*{}", q.a(), q.b(), q.c(), q.d());
    }

private:
    Complex m_z0;
    Complex m_z1;
};

} // namespace iheay::math
