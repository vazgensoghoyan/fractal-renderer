// fractal/inl/fractal_renderer_builder.inl

#include <stdexcept>
#include <cassert>

namespace iheay::fractal {

// private constructor

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>::FractalRendererBuilder(
    double viewport_width, 
    math::Complex viewport_min,
    int max_iter,
    double escape_radius,
    IterationFunc iterate,
    InitialFunc init,
    ParamFunc param,
    Colorizer colorizer
)
: m_viewport_width(viewport_width)
, m_viewport_min(viewport_min)
, m_max_iter(max_iter), m_escape_radius(escape_radius)
, m_iterate(iterate), m_init(init)
, m_param(param), m_colorizer(colorizer) {}

// getting builder with initial values for parameters

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer> FractalRendererBuilder<Colorizer>::get_builder() {

    return FractalRendererBuilder<Colorizer>(
        3.0, math::Complex::Algebraic(-2, -1.5),
        300, 2,
        [](const math::Complex& z, const math::Complex& c) { return z * z + c; },
        [](const math::Complex& pixel) { return pixel; },
        [](const math::Complex&) { return math::Complex::Zero(); },
        Colorizer{}
    );
}

// ---------------- build ----------------

template <ColorizerConcept Colorizer>
FractalRenderer<Colorizer> FractalRendererBuilder<Colorizer>::build() const {
    
    return FractalRenderer<Colorizer>(
        m_viewport_width,
        m_viewport_min,
        m_max_iter,
        m_escape_radius,
        m_iterate,
        m_init,
        m_param,
        m_colorizer
    );
}

// setting parameters

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_viewport(Viewport viewport) {
    return set_viewport_width(
                viewport.max.real() - viewport.min.real()
            ).set_viewport_min(viewport.min);
}

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_viewport_width(double width) {
    if (width <= 0)
        throw std::runtime_error("Invalid viewport width");
    m_viewport_width = width;
    return *this;
}

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_viewport_min(math::Complex min) {
    m_viewport_min = min;
    return *this;
}

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_max_iter(int max_iter) {
    if (max_iter <= 0)
        throw std::runtime_error("Invalid max_iter");
    m_max_iter = max_iter;
    return *this;
}

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_escape_radius(double escape_radius) {
    if (escape_radius <= 0)
        throw std::runtime_error("Invalid escape_radius");
    m_escape_radius = escape_radius;
    return *this;
}

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_iteration_func(IterationFunc iterate) {
    m_iterate = std::move(iterate);
    return *this;
}

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_initial_func(InitialFunc init) {
    m_init = std::move(init);
    return *this;
}

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_param_func(ParamFunc param) {
    m_param = std::move(param);
    return *this;
}

template <ColorizerConcept Colorizer>
FractalRendererBuilder<Colorizer>&
FractalRendererBuilder<Colorizer>::set_colorizer(Colorizer colorizer) {
    m_colorizer = std::move(colorizer);
    return *this;
}

} // namespace iheay::fractal
