#include "fractal/fractal_renderer_builder.hpp"

using namespace iheay::math;
using namespace iheay::fractal;

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
, m_viewport_height(viewport_width * height / width)
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
    m_width = width;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_image_height(int height) {
    m_height = height;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_viewport_width(double width) {
    m_viewport_width = width;
    m_viewport_height = width * m_height / m_width;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_viewport_min(math::Complex min) {
    m_viewport_min = min;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_max_iter(int max_iter) {
    m_max_iter = max_iter;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_escape_radius(double escape_radius) {
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
