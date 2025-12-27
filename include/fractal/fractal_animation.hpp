#pragma once // fractal/fractal_animation.hpp

#include "fractal/fractal_camera.hpp"

namespace iheay::fractal {

struct FractalKeyframe {
    FractalCamera camera;
    FractalConfig config;
    math::Complex julia_c;
};

FractalKeyframe interpolate(
    const FractalKeyframe& a,
    const FractalKeyframe& b,
    double t
);

} // namespace iheay::fractal
