#pragma once // fractal/fractal_animation.hpp

#include "fractal/fractal_structures.hpp"

namespace iheay::fractal {

struct FractalKeyframe {
    Viewport viewport;
    FractalConfig config;
    math::Complex julia_c;
};

FractalKeyframe interpolate(
    const FractalKeyframe& a,
    const FractalKeyframe& b,
    double t
);

} // namespace iheay::fractal
