#pragma once // fractal/fractal_structures.hpp

#include "math/complex.hpp"
#include <functional>

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

} // namespace iheay::fractal