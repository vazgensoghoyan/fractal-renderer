#include "fractal/fractal_renderer_builder.hpp"
#include <stdexcept>

using namespace iheay::math;
using namespace iheay::fractal;

// private calculation viewport height

inline double FractalRendererBuilder::recalc_viewport_height() const {
    return m_viewport_width * m_height / m_width;
}

// private constructor

FractalRendererBuilder::FractalRendererBuilder(
    int width, int height,
    double viewport_width, 
    math::Complex viewport_min,
    int max_iter,
    double escape_radius,
    IterationFunc iterate,
    InitialFunc init,
    ParamFunc param
) 
: m_width(width), m_height(height)
, m_viewport_width(viewport_width)
, m_viewport_height(recalc_viewport_height())
, m_viewport_min(viewport_min)
, m_max_iter(max_iter), m_escape_radius(escape_radius)
, m_iterate(iterate), m_init(init), m_param(param) {}

// static getting builder

FractalRendererBuilder FractalRendererBuilder::get_builder() {
    // setting up init values for all properties
    return FractalRendererBuilder(
        256, 256,
        3,
        Complex::Algebraic(-2.0, -1.5),
        300, 2.0,
        [](auto& z, auto& c) { return z * z + c; },
        [](auto& pixel) { return pixel; },
        [](auto&) { return Complex::Zero(); }
    );
}

// setting properties for constructor

FractalRenderer FractalRendererBuilder::build() const {
    return FractalRenderer(
        m_width, m_height,
        m_viewport_width, m_viewport_min,
        m_max_iter, m_escape_radius,
        m_iterate, m_init, m_param
    );
}

FractalRendererBuilder& FractalRendererBuilder::set_image_width(int width) {
    if (width <= 0)
        throw std::runtime_error("Invalid width given in fractal renderer builder");
    m_width = width;
    m_viewport_height = recalc_viewport_height();
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_image_height(int height) {
    if (height <= 0)
        throw std::runtime_error("Invalid height given in fractal renderer builder");
    m_height = height;
    m_viewport_height = recalc_viewport_height();
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_viewport(Viewport viewport) {
    return set_viewport_width(
                viewport.max.real() - viewport.min.real()
            ).set_viewport_min(viewport.min);
}

FractalRendererBuilder& FractalRendererBuilder::set_viewport_width(double width) {
    if (width <= 0)
        throw std::runtime_error("Invalid viewport width given in fractal renderer builder");
    m_viewport_width = width;
    m_viewport_height = recalc_viewport_height();
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_viewport_min(math::Complex min) {
    m_viewport_min = min;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_max_iter(int max_iter) {
    if (max_iter <= 0)
        throw std::runtime_error("Invalid max_iter parameter given in fractal renderer builder");
    m_max_iter = max_iter;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_escape_radius(double escape_radius) {
    if (escape_radius <= 0)
        throw std::runtime_error("Invalid escape_radius parameter given in fractal renderer builder");
    m_escape_radius = escape_radius;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_iteration_func(IterationFunc iterate) {
    m_iterate = iterate;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_initial_func(InitialFunc init) {
    m_init = init;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_param_func(ParamFunc param) {
    m_param = param;
    return *this;
}
