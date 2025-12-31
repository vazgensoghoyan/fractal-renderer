#pragma once // fractal/fractal_renderer.hpp

#include "fractal/fractal_structures.hpp"
#include "math/complex.hpp"
#include "bmp/bmp.hpp"
#include "rasterizer/interface_pixeled.hpp"

namespace iheay::fractal {

class FractalRenderer {
public:
    FractalRenderer(
        int width, int height,
        Viewport viewport,
        FractalConfig config,
        IterationFunc iterate,
        InitialFunc init,
        ParamFunc param
    );

    bmp::Bmp render() const;

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
