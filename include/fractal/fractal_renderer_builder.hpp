#pragma once // fractal/fractal_renderer_builder.hpp

#include "math/complex.hpp"
#include "fractal/fractal_renderer.hpp"

namespace iheay::fractal {

class FractalRendererBuilder {
public:
    FractalRendererBuilder(
        int width, int height,
        double viewport_width, 
        math::Complex viewport_min,
        int max_iter,
        double escape_radius,
        IterationFunc iterate,
        InitialFunc init,
        ParamFunc param
    );

    static FractalRendererBuilder get_builder();

    FractalRenderer build() const;

    FractalRendererBuilder& set_image_width(int width);
    FractalRendererBuilder& set_image_height(int height);

    FractalRendererBuilder& set_viewport_width(double width);
    FractalRendererBuilder& set_viewport_min(math::Complex min);

    FractalRendererBuilder& set_max_iter(int max_iter);
    FractalRendererBuilder& set_escape_radius(double escape_radius);

    FractalRendererBuilder& set_iteration_func(IterationFunc iterate);
    FractalRendererBuilder& set_initial_func(InitialFunc init);
    FractalRendererBuilder& set_param_func(ParamFunc param);

private:
    int m_width;
    int m_height;

    double m_viewport_width;
    double m_viewport_height;
    math::Complex m_viewport_min;

    int m_max_iter;
    double m_escape_radius;

    IterationFunc m_iterate;
    InitialFunc m_init;
    ParamFunc m_param;
};

} // namespace iheay::fractal
