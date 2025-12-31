#pragma once // fractal/fractal_renderer_builder.hpp

#include "math/complex.hpp"
#include "fractal/fractal_renderer.hpp"

namespace iheay::fractal {

class FractalRendererBuilder {
public:
    FractalRendererBuilder(
        int width, int height,
        Viewport viewport,
        FractalConfig config,
        IterationFunc iterate,
        InitialFunc init,
        ParamFunc param
    );

    static FractalRendererBuilder get_builder();

    FractalRenderer build() const;

    FractalRendererBuilder& set_image_width(int width);
    FractalRendererBuilder& set_image_height(int height);

    FractalRendererBuilder& set_viewport(Viewport viewport);
    FractalRendererBuilder& set_viewport_max(math::Complex max);
    FractalRendererBuilder& set_viewport_min(math::Complex min);

    FractalRendererBuilder& set_max_iter(int max_iter);
    FractalRendererBuilder& set_escape_radius(double escape_radius);

    FractalRendererBuilder& set_iteration_func(IterationFunc iterate);
    FractalRendererBuilder& set_initial_func(InitialFunc init);
    FractalRendererBuilder& set_param_func(ParamFunc param);

private:
    int m_width;
    int m_height;
    Viewport m_viewport;
    FractalConfig m_config;
    IterationFunc m_iterate;
    InitialFunc m_init;
    ParamFunc m_param;
};

} // namespace iheay::fractal
