#pragma once // fractal/fractal_structures.hpp

#include "math/complex.hpp"
#include <functional>

namespace iheay::fractal {

template <typename Colorizer>
concept ColorizerConcept =
requires(Colorizer c, double mu, int max_iter) {
    typename Colorizer::pixel_type;
    { c(mu, max_iter) } -> std::same_as<typename Colorizer::pixel_type>;
};

struct FractalConfig {
    int max_iter;
    double escape_radius;
};

struct Viewport {
    double width;
    math::Complex center;
};

using IterationFunc = std::function<math::Complex(const math::Complex& z, const math::Complex& c)>;

using InitialFunc = std::function<math::Complex(const math::Complex& pixel)>;

using ParamFunc = std::function<math::Complex(const math::Complex& pixel)>;

} // namespace iheay::fractal