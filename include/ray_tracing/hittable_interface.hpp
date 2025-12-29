#pragma once // ray_tracing/hittable_interface.hpp

#include "math/vec3.hpp"
#include "math/ray.hpp"
#include <optional>

namespace iheay::ray_tracing {

struct HitRecord {
    math::Vec3 p;
    math::Vec3 normal;
    double t;
};

class IHittable {
public:
    virtual std::optional<HitRecord> hit(
        const math::Ray& ray, 
        double ray_tmin, 
        double ray_tmax
    ) const = 0;
};

} // namespace iheay::ray_tracing
