#pragma once // ray_tracing/objects/sphere.hpp

#include "ray_tracing/hittable_interface.hpp"

namespace iheay::ray_tracing::objects {

class Sphere : public IHittable {
public:
    Sphere(const math::Vec3& center, double radius);

    std::optional<HitRecord> hit(
        const math::Ray& ray, 
        double ray_tmin, 
        double ray_tmax
    ) const override;

private:
    math::Vec3 m_center;
    double m_radius;
};

} // namespace iheay::ray_tracing::objects
