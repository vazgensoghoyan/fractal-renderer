#pragma once // fractal/fractal.hpp

#include <functional>
#include "math/complex.hpp"
#include "bmp/bmp.hpp"
#include "rasterizer/interface_pixeled.hpp"

namespace iheay::fractal {

struct FractalConfig {
    int max_iter = 300;
    double escape_radius = 2.0;
};

struct Viewport {
    math::Complex min;
    math::Complex max;
};

using IterationFunc = std::function<math::Complex(const math::Complex& z, const math::Complex& c)>;

using InitialFunc = std::function<math::Complex(const math::Complex& pixel)>;

using ParamFunc = std::function<math::Complex(const math::Complex& pixel)>;

iheay::bmp::Bmp render_complex_fractal(
    int width, int height,
    const Viewport& view,
    IterationFunc iterate,
    InitialFunc initial,
    ParamFunc parameter,
    const FractalConfig& cfg
);

} // namespace iheay::fractal
