#pragma once

#include <cmath>
#include <ostream>

namespace iheay::math {

class Vec3 {
public:
    constexpr Vec3() : m_x(0), m_y(0), m_z(0) { }
    constexpr Vec3(double x, double y, double z) : m_x(x), m_y(y), m_z(z) { }

    constexpr double x() const { return m_x; }
    constexpr double y() const { return m_y; }
    constexpr double z() const { return m_z; }

    double length() const { return std::sqrt(length_squared()); }
    double length_squared() const { return m_x * m_x + m_y * m_y + m_z * m_z; }

    Vec3 normalized() const {
        double len = length();
        if (len == 0) return Vec3(0,0,0);
        return *this / len;
    }

    Vec3 operator-() const { return Vec3(-m_x, -m_y, -m_z); }

    Vec3 operator+(const Vec3& o) const { return Vec3(m_x + o.m_x, m_y + o.m_y, m_z + o.m_z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(m_x - o.m_x, m_y - o.m_y, m_z - o.m_z); }
    Vec3 operator*(double scalar) const { return Vec3(m_x * scalar, m_y * scalar, m_z * scalar); }
    Vec3 operator/(double scalar) const { return Vec3(m_x / scalar, m_y / scalar, m_z / scalar); }

    Vec3& operator+=(const Vec3& o) { m_x += o.m_x; m_y += o.m_y; m_z += o.m_z; return *this; }
    Vec3& operator-=(const Vec3& o) { m_x -= o.m_x; m_y -= o.m_y; m_z -= o.m_z; return *this; }
    Vec3& operator*=(double scalar) { m_x *= scalar; m_y *= scalar; m_z *= scalar; return *this; }
    Vec3& operator/=(double scalar) { m_x /= scalar; m_y /= scalar; m_z /= scalar; return *this; }

    double dot(const Vec3& o) const { return m_x * o.m_x + m_y * o.m_y + m_z * o.m_z; }
    Vec3 cross(const Vec3& o) const {
        return Vec3(
            m_y * o.m_z - m_z * o.m_y,
            m_z * o.m_x - m_x * o.m_z,
            m_x * o.m_y - m_y * o.m_x
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec3& v) {
        return os << '(' << v.m_x << ", " << v.m_y << ", " << v.m_z << ')';
    }

private:
    double m_x, m_y, m_z;
};

inline Vec3 operator*(double scalar, const Vec3& v) {
    return v * scalar;
}

} // namespace iheay::math
