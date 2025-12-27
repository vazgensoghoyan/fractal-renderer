#include "fractal/fractal_camera.hpp"

using namespace iheay::fractal;
using namespace iheay::math;

Viewport iheay::fractal::make_viewport(
    const FractalCamera& cam,
    int image_width,
    int image_height
) {
    double aspect = static_cast<double>(image_height) / image_width;

    double half_w = cam.scale;
    double half_h = cam.scale * aspect;

    return {
        Complex::Algebraic(
            cam.center.real() - half_w,
            cam.center.imag() - half_h
        ),
        Complex::Algebraic(
            cam.center.real() + half_w,
            cam.center.imag() + half_h
        )
    };
}
