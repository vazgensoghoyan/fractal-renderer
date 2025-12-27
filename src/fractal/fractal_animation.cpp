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

    // ⚠️ scale — ЭКСПОНЕНЦИАЛЬНАЯ интерполяция
    out.camera.scale = a.camera.scale * std::pow(b.camera.scale / a.camera.scale, t);

    out.config.max_iter = static_cast<int>(
        lerp(a.config.max_iter, b.config.max_iter, t)
    );

    out.config.escape_radius =
        lerp(a.config.escape_radius, b.config.escape_radius, t);

    out.julia_c = lerp(a.julia_c, b.julia_c, t);

    return out;
}
