#pragma once // math/ray.hpp

#include "math/vec3.hpp"

namespace iheay::math {

class Ray {
public:
    Ray() = delete;

    Ray(const Vec3& origin, const Vec3& direction) noexcept
        : m_origin(origin)
        , m_direction(direction.normalized())
    {}

    [[nodiscard]] constexpr const Vec3& origin() const noexcept { return m_origin; }
    [[nodiscard]] constexpr const Vec3& direction() const noexcept { return m_direction; }

    [[nodiscard]] constexpr Vec3 at(double t) const noexcept {
        return m_origin + t * m_direction;
    }

private:
    Vec3 m_origin;
    Vec3 m_direction;
};

} // namespace iheay::math
