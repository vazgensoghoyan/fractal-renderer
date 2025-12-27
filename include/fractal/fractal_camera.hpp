#pragma once // fractal/fractal_camera.hpp

#include "fractal/fractal.hpp"
#include "math/complex.hpp"

namespace iheay::fractal {

struct FractalCamera {
    math::Complex center;
    double scale;
};

Viewport make_viewport(
    const FractalCamera& cam,
    int width,
    int height
);

}
