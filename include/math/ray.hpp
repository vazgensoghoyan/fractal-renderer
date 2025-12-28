#pragma once // math/ray.hpp

#include "math/vec3.hpp"

namespace iheay::math {

class Ray {
public:
    Ray() {}
    Ray(Vec3 origin, Vec3 direction) : m_origin(origin), m_direction(direction) {}

    const Vec3& origin() const { return m_origin; }
    const Vec3& direction() const { return m_direction; }

    Vec3 at(double t) const { return m_origin + t * m_direction; }

private:
    Vec3 m_origin;
    Vec3 m_direction;
};

} // namespace iheay::math
