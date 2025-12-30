#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"

#include "fractal/fractal_renderer.hpp"
#include "fractal/fractal_camera.hpp"
#include "fractal/fractal_animation.hpp"

#include "math/complex.hpp"
#include "math/vec3.hpp"
#include "math/ray.hpp"

#include "ray_tracing/objects/sphere.hpp"

#include "utils/logger.hpp"

#include <format>
#include <omp.h>

using namespace iheay::bmp;
using namespace iheay::math;
using namespace iheay::fractal;
using namespace iheay::ray_tracing;

BgrPixel vec3_to_pixel(const Vec3& vec) {
    if (vec.x() < 0 || vec.y() < 0 || vec.z() < 0)
        throw std::runtime_error("Can't transform from math::Vec3 to bmp::Pixel");

    Vec3 vec_norm = vec.normalized();

    uint8_t r = uint8_t(255.999 * vec_norm.x());
    uint8_t g = uint8_t(255.999 * vec_norm.y());
    uint8_t b = uint8_t(255.999 * vec_norm.z());

    return { b, g, r };
}

BgrPixel ray_color(const Ray& ray) {
    objects::Sphere sphere( {0,0,-1}, 0.5 );
    std::optional<HitRecord> rec = sphere.hit(ray, 0, 100000000);

    if (rec.has_value()) {
        HitRecord rec_val = rec.value();
        Vec3 normal = rec_val.normal;
        Vec3 color = 0.5 * (normal + Vec3(1, 1, 1));
        return vec3_to_pixel(color);
    }

    Vec3 dir_norm = ray.direction().normalized();
    double a = (dir_norm.y() + 1) / 2;
    Vec3 color = (1 - a) * Vec3(1, 1, 1) + a * Vec3(0.5, 0.7, 1);
    return vec3_to_pixel(color);
}

int main() {
    try {

        double aspect_ratio = 16.0 / 9.0;

        int image_width = 400;
        int image_height = int(image_width / aspect_ratio);

        double viewport_height = 2.0;
        double viewport_width = viewport_height * image_width / image_height;

        double focal_length = 1.0;
        Vec3 camera_center = Vec3(0, 0, 0);

        Vec3 viewport_u = Vec3(viewport_width, 0, 0);
        Vec3 viewport_v = Vec3(0, -viewport_height, 0);

        Vec3 pixel_delta_u = viewport_u / image_width;
        Vec3 pixel_delta_v = viewport_v / image_height;

        Vec3 viewport_upper_left = camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        Vec3 pixel_00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) / 2;

        Bmp image = Bmp::empty(image_width, image_height);

        for (int i = 0; i < image_width; ++i) {
            for (int j = 0; j < image_height; ++j) {
                Vec3 pixel_center = pixel_00_loc + i * pixel_delta_u + j * pixel_delta_v;
                Vec3 ray_direction = pixel_center - camera_center;

                Ray ray = Ray(camera_center, ray_direction);

                image.set_pixel(i, j, ray_color(ray));
            }
        }

        io::save(image, "../ray_tracing.bmp");

    } catch (const std::exception& e) {

        LOG_ERROR("Exception: {}", e.what());
        return 1;

    }

    return 0;
}
