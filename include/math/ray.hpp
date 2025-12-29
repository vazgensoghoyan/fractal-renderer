#pragma once // math/ray.hpp

#include "math/vec3.hpp"

#include <cassert>

namespace iheay::math {

class Ray {
public:
    Ray() = delete;

    constexpr Ray(const Vec3& origin, const Vec3& direction) noexcept
        : m_origin(origin),
          m_direction(normalize_checked(direction))
    {}

    [[nodiscard]] constexpr const Vec3& origin() const noexcept {
        return m_origin;
    }

    [[nodiscard]] constexpr const Vec3& direction() const noexcept {
        return m_direction;
    }

    [[nodiscard]] constexpr Vec3 at(double t) const noexcept {
        return m_origin + t * m_direction;
    }

private:
    static constexpr Vec3 normalize_checked(const Vec3& v) noexcept {
        const double len = v.length_squared();
        assert(len > 0 && "Ray direction must be non-zero");
        return v / std::sqrt(len);
    }

private:
    Vec3 m_origin;
    Vec3 m_direction;
};

} // namespace iheay::math
