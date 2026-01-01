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
        double viewport_width, 
        math::Complex viewport_min,
        int max_iter,
        double escape_radius,
        IterationFunc iterate,
        InitialFunc init,
        ParamFunc param
    );

    [[nodiscard]] bmp::Bmp render() const;

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
