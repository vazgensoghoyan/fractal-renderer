#include "ray_tracing/objects/sphere.hpp"

using namespace iheay::ray_tracing;
using namespace iheay::ray_tracing::objects;
using namespace iheay::math;

Sphere::Sphere(const Vec3& center, double radius) : m_center(center), m_radius(radius)
{ }

std::optional<HitRecord> Sphere::hit(const Ray& ray, double ray_tmin, double ray_tmax) const {
    Vec3 oc = m_center - ray.origin();

    double a = ray.direction().length_squared();
    double b = Vec3::dot(ray.direction(), oc);
    double c = oc.length_squared()- m_radius * m_radius;

    double discriminant = b * b - a * c;
    if (discriminant < 0)
        return {};

    double discr_sqrt = std::sqrt(discriminant);

    double root = (b - discr_sqrt) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (b + discr_sqrt) / a;
        if (root <= ray_tmin || ray_tmax <= root)
            return {};
    }

    Vec3 p = ray.at(root);

    return HitRecord(p, (p - m_center) / m_radius, root);
}
