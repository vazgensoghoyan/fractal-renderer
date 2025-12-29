#pragma once // math/quaternion.hpp

#include "math/complex.hpp"
#include "math/vec3.hpp"
#include <cmath>
#include <ostream>
#include <format>
#include <stdexcept>

namespace iheay::math {

class Quaternion {
public:
    Quaternion(double a, double b, double c, double d)
        : m_z0(Complex::Algebraic(a, b))
        , m_z1(Complex::Algebraic(c, d)) {}

    Quaternion(Complex z0, Complex z1) noexcept : m_z0(z0), m_z1(z1) {}

    [[nodiscard]] constexpr double a() const noexcept { return m_z0.real(); }
    [[nodiscard]] constexpr double b() const noexcept { return m_z0.imag(); }
    [[nodiscard]] constexpr double c() const noexcept { return m_z1.real(); }
    [[nodiscard]] constexpr double d() const noexcept { return m_z1.imag(); }

    [[nodiscard]] const Complex& z0() const noexcept { return m_z0; }
    [[nodiscard]] const Complex& z1() const noexcept { return m_z1; }

    [[nodiscard]] double modulus() const noexcept { return std::hypot(m_z0.modulus(), m_z1.modulus()); }

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

    [[nodiscard]] Quaternion inverse() const {
        double modul_squared = modulus();
        modul_squared *= modul_squared;
        if (modul_squared == 0) throw std::runtime_error("Inverse of zero quaternion");
        return Quaternion(~m_z0 / modul_squared, -m_z1 / modul_squared);
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

    static Vec3 rotate_point(const Vec3& point, const Vec3& axis, double phi) {
        if (axis.length_squared() == 0) 
            throw std::invalid_argument("Rotation axis is zero");
        double half_phi = phi / 2;
        Vec3 s = axis.normalized() * sin(half_phi);
        Quaternion q(std::cos(half_phi), s.x(), s.y(), s.z());
        Quaternion v(0, point.x(), point.y(), point.z());
        Quaternion rotated = q * v * q.inverse();
        return Vec3(rotated.b(), rotated.c(), rotated.d());
    }

    static Vec3 rotate_x(const Vec3& point, double phi) { return rotate_point(point, {1,0,0}, phi); }
    static Vec3 rotate_y(const Vec3& point, double phi) { return rotate_point(point, {0,1,0}, phi); }
    static Vec3 rotate_z(const Vec3& point, double phi) { return rotate_point(point, {0,0,1}, phi); }

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
        return os << std::format("{} + i*{} + j*{} + k*{}", q.a(), q.b(), q.c(), q.d());
    }

private:
    Complex m_z0;
    Complex m_z1;
};

} // namespace iheay::math
