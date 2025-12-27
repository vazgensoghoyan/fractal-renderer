#pragma once // fractal/fractal_camera.hpp

#include "fractal/fractal.hpp"
#include "math/complex.hpp"

namespace iheay::fractal {

struct FractalCamera {
    math::Complex center;
    double scale; // половина ширины по real-оси
};

Viewport make_viewport(
    const FractalCamera& cam,
    int image_width,
    int image_height
);

} // namespace iheay::fractal
