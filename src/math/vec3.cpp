#include "math/vec3.hpp"

#include <cmath>

using namespace iheay::math;

double Vec3::length() const {
    return std::sqrt(length_squared());
}

double Vec3::length_squared() const {
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

Vec3 Vec3::normalized() const {
    double len = length();
    if (len == 0.0)
        return Vec3(0.0, 0.0, 0.0);
    return *this / len;
}

double Vec3::dot(const Vec3& v1, const Vec3& v2) { 
    return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z; 
}

Vec3 Vec3::cross(const Vec3& v1, const Vec3& v2) {
    return Vec3(
        v1.m_y * v2.m_z - v1.m_z * v2.m_y,
        v1.m_z * v2.m_x - v1.m_x * v2.m_z,
        v1.m_x * v2.m_y - v1.m_y * v2.m_x
    );
}

std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    return os << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')';
}
