#pragma once // fractal/fractal_renderer.hpp

#include "fractal/fractal_structures.hpp"
#include "math/complex.hpp"
#include "bmp/bmp.hpp"
#include "rasterizer/interface_pixeled.hpp"

namespace iheay::fractal {

class FractalRenderer {
public:

    // make private
    FractalRenderer(
        int width, int height,
        Viewport viewport,
        IterationFunc iterate,
        InitialFunc init,
        ParamFunc param,
        FractalConfig config
    );

    // TODO

    iheay::bmp::Bmp render();

    friend class FractalRendererBuilder;

private:
    int m_width, m_height;
    Viewport m_viewport;
    IterationFunc m_iterate;
    InitialFunc m_init;
    ParamFunc m_param;
    FractalConfig m_config;
};

} // namespace iheay::fractal
