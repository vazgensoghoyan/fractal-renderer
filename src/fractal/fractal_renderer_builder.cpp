#include "fractal/fractal_renderer_builder.hpp"

using namespace iheay::math;
using namespace iheay::fractal;

FractalRendererBuilder::FractalRendererBuilder(
    int width, int height,
    Viewport viewport,
    FractalConfig config,
    IterationFunc iterate,
    InitialFunc init,
    ParamFunc param
) 
: m_width(width), m_height(height)
, m_viewport(viewport), m_config(config)
, m_iterate(iterate), m_init(init), m_param(param) {}

// static getting builder

FractalRendererBuilder FractalRendererBuilder::get_builder() {
    // setting up init values for all properties
    return FractalRendererBuilder(
        256, 256,
        Viewport{
            Complex::Algebraic(-2.0, -1.5),
            Complex::Algebraic(1.0, 1.5)
        },
        {300, 2.0},
        [](auto& z, auto& c) { return z * z + c; },
        [](auto& pixel) { return pixel; },
        [](auto&) { return Complex::Zero(); }
    );
}

// setting properties for constructor

FractalRenderer FractalRendererBuilder::build() const {
    return FractalRenderer(
        m_width, m_height,
        m_viewport, m_config,
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

FractalRendererBuilder& FractalRendererBuilder::set_viewport(Viewport viewport) {
    m_viewport = viewport;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_viewport_max(math::Complex max) {
    m_viewport.max = max;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_viewport_min(math::Complex min) {
    m_viewport.min = min;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_max_iter(int max_iter) {
    m_config.max_iter = max_iter;
    return *this;
}

FractalRendererBuilder& FractalRendererBuilder::set_escape_radius(double escape_radius) {
    m_config.escape_radius = escape_radius;
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
