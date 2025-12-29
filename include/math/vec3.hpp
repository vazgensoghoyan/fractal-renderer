#pragma once // math/vec3.hpp

#include <cmath>
#include <cassert>
#include <ostream>

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

    [[nodiscard]] Vec3 normalized() const noexcept {
        const double len2 = length_squared();
        assert(len2 > 0 && "Cannot normalize 0-vector");
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

    [[nodiscard]] constexpr Vec3 operator/(double scalar) const noexcept {
        assert(scalar != 0 && "Division by zero in Vec3");
        return Vec3(m_x / scalar, m_y / scalar, m_z / scalar);
    }

    constexpr Vec3& operator+=(const Vec3& o) noexcept {
        m_x += o.m_x;
        m_y += o.m_y;
        m_z += o.m_z;
        return *this;
    }

    constexpr Vec3& operator-=(const Vec3& o) noexcept {
        m_x -= o.m_x;
        m_y -= o.m_y;
        m_z -= o.m_z;
        return *this;
    }

    constexpr Vec3& operator*=(double scalar) noexcept {
        m_x *= scalar;
        m_y *= scalar;
        m_z *= scalar;
        return *this;
    }

    constexpr Vec3& operator/=(double scalar) noexcept {
        assert(scalar != 0 && "Division by zero in Vec3");
        m_x /= scalar;
        m_y /= scalar;
        m_z /= scalar;
        return *this;
    }

    [[nodiscard]] static constexpr double dot(const Vec3& v, const Vec3& w) noexcept {
        return v.m_x * w.m_x + v.m_y * w.m_y + v.m_z * w.m_z;
    }

    [[nodiscard]] static constexpr Vec3 cross(const Vec3& v, const Vec3& w) noexcept {
        return Vec3(
            v.m_y * w.m_z - v.m_z * w.m_y,
            v.m_z * w.m_x - v.m_x * w.m_z,
            v.m_x * w.m_y - v.m_y * w.m_x
        );
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
