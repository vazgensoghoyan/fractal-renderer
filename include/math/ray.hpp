#pragma once // math/ray.hpp

#include "math/vec3.hpp"

namespace iheay::math {

class Ray {
public:
    // constructors (no default, cant have direction = 0)

    Ray() = delete;

    Ray(const Vec3& origin, const Vec3& direction)
        : m_origin(origin)
        , m_direction(direction.normalized()) // if 0 -> exception
    {}

    // properties

    [[nodiscard]] constexpr const Vec3& origin() const noexcept { return m_origin; }
    [[nodiscard]] constexpr const Vec3& direction() const noexcept { return m_direction; }

    // calculating point at the ray

    [[nodiscard]] constexpr Vec3 at(double t) const noexcept {
        return m_origin + t * m_direction;
    }

private:
    Vec3 m_origin;
    Vec3 m_direction;
};

} // namespace iheay::math
