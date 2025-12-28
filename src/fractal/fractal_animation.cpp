#include "fractal/fractal_animation.hpp"

using namespace iheay::fractal;
using namespace iheay::math;

static double lerp(double a, double b, double t) {
    return a * (1.0 - t) + b * t;
}

static Complex lerp(const Complex& a, const Complex& b, double t) {
    return Complex::Algebraic(
        lerp(a.real(), b.real(), t),
        lerp(a.imag(), b.imag(), t)
    );
}

FractalKeyframe iheay::fractal::interpolate(
    const FractalKeyframe& a,
    const FractalKeyframe& b,
    double t
) {
    FractalKeyframe out;

    out.camera.center = lerp(a.camera.center, b.camera.center, t);

    //out.camera.scale = a.camera.scale * std::pow(b.camera.scale / a.camera.scale, t);
    out.camera.scale = lerp(a.camera.scale, b.camera.scale, t);

    int base_iter = 200;
    double iter_factor = 50.0;

    double zoom = 1.0 / out.camera.scale;

    out.config.max_iter = base_iter + static_cast<int>(iter_factor * std::log2(zoom));

    out.config.escape_radius = a.config.escape_radius;

    out.julia_c = lerp(a.julia_c, b.julia_c, t);

    return out;
}
