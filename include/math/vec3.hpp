#pragma once // math/vec3.hpp

#include <cmath>
#include <ostream>
#include <concepts>

namespace iheay::math {

class Vec3 {
public:
    constexpr Vec3() noexcept : m_x(0.0), m_y(0.0), m_z(0.0) {}
    
    constexpr Vec3(double x, double y, double z) noexcept : m_x(x), m_y(y), m_z(z) {}

    [[nodiscard]] constexpr double x() const noexcept { return m_x; }
    [[nodiscard]] constexpr double y() const noexcept { return m_y; }
    [[nodiscard]] constexpr double z() const noexcept { return m_z; }

    [[nodiscard]] constexpr double length_squared() const noexcept {
        return m_x * m_x + m_y * m_y + m_z * m_z;
    }

    [[nodiscard]] double length() const noexcept {
        return std::sqrt(length_squared());
    }

    [[nodiscard]] Vec3 normalized() const {
        const double len2 = length_squared();
        if (len2 == 0)
            throw std::runtime_error("Cannot normalize zero vector");
        return *this / std::sqrt(len2);
    }

    [[nodiscard]] constexpr Vec3 operator-() const noexcept {
        return Vec3(-m_x, -m_y, -m_z);
    }

    [[nodiscard]] constexpr Vec3 operator+(const Vec3& o) const noexcept {
        return Vec3(m_x + o.m_x, m_y + o.m_y, m_z + o.m_z);
    }

    [[nodiscard]] constexpr Vec3 operator-(const Vec3& o) const noexcept {
        return Vec3(m_x - o.m_x, m_y - o.m_y, m_z - o.m_z);
    }

    [[nodiscard]] constexpr Vec3 operator*(double scalar) const noexcept {
        return Vec3(m_x * scalar, m_y * scalar, m_z * scalar);
    }

    [[nodiscard]] constexpr Vec3 operator/(double scalar) const {
        if (scalar == 0) throw std::invalid_argument("Division by zero in Vec3");
        return Vec3(m_x / scalar, m_y / scalar, m_z / scalar);
    }

    constexpr Vec3& operator+=(const Vec3& o) noexcept {
        m_x += o.m_x; m_y += o.m_y; m_z += o.m_z; return *this;
    }

    constexpr Vec3& operator-=(const Vec3& o) noexcept {
        m_x -= o.m_x; m_y -= o.m_y; m_z -= o.m_z; return *this;
    }

    constexpr Vec3& operator*=(double scalar) noexcept {
        m_x *= scalar; m_y *= scalar; m_z *= scalar; return *this;
    }

    constexpr Vec3& operator/=(double scalar) {
        if (scalar == 0) throw std::invalid_argument("Division by zero in Vec3");
        m_x /= scalar; m_y /= scalar; m_z /= scalar; return *this;
    }

    [[nodiscard]] static constexpr double dot(const Vec3& a, const Vec3& b) noexcept {
        return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
    }

    [[nodiscard]] static constexpr Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
        return Vec3(
            a.m_y * b.m_z - a.m_z * b.m_y,
            a.m_z * b.m_x - a.m_x * b.m_z,
            a.m_x * b.m_y - a.m_y * b.m_x
        );
    }

    [[nodiscard]] constexpr bool operator==(const Vec3& o) const noexcept {
        return m_x == o.m_x && m_y == o.m_y && m_z == o.m_z;
    }

    [[nodiscard]] constexpr bool operator!=(const Vec3& o) const noexcept {
        return !(*this == o);
    }

private:
    double m_x;
    double m_y;
    double m_z;
};

[[nodiscard]] constexpr Vec3 operator*(double scalar, const Vec3& v) noexcept {
    return v * scalar;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    return os << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')';
}

} // namespace iheay::math
