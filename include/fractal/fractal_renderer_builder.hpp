#pragma once // fractal/fractal_renderer_builder.hpp

#include "math/complex.hpp"
#include "fractal/fractal_renderer.hpp"

namespace iheay::fractal {

template <ColorizerConcept Colorizer>
class FractalRendererBuilder {
public:
    static FractalRendererBuilder get_builder();

    FractalRenderer<Colorizer> build() const;

    FractalRendererBuilder& set_viewport(Viewport);
    FractalRendererBuilder& set_viewport_width(double);
    FractalRendererBuilder& set_viewport_min(math::Complex);

    FractalRendererBuilder& set_max_iter(int);
    FractalRendererBuilder& set_escape_radius(double);

    FractalRendererBuilder& set_iteration_func(IterationFunc iterate);
    FractalRendererBuilder& set_initial_func(InitialFunc initial);
    FractalRendererBuilder& set_param_func(ParamFunc param);

    FractalRendererBuilder& set_colorizer(Colorizer);

private:
    FractalRendererBuilder(
        double viewport_width,
        math::Complex viewport_min,
        int max_iter,
        double escape_radius,
        IterationFunc iterate,
        InitialFunc init,
        ParamFunc param,
        Colorizer colorizer
    );

private:
    double m_viewport_width;
    math::Complex m_viewport_min;

    int m_max_iter;
    double m_escape_radius;

    IterationFunc m_iterate;
    InitialFunc m_init;
    ParamFunc m_param;

    Colorizer m_colorizer;
};

} // namespace iheay::fractal

#include "inl/fractal_renderer_builder.inl"
