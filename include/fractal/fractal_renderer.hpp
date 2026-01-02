#pragma once // fractal/fractal_renderer.hpp

#include "rasterizer/pixeled_concept.hpp"
#include "fractal/fractal_structures.hpp"
#include "math/complex.hpp"

namespace iheay::fractal {

template <ColorizerConcept Colorizer>
class FractalRenderer {
public:
    FractalRenderer(
        double viewport_width,
        math::Complex viewport_min,
        int max_iter,
        double escape_radius,
        IterationFunc iterate,
        InitialFunc init,
        ParamFunc param,
        Colorizer colorizer
    );

    template <raster::PixeledImage Image>
    void render(Image& image) const;

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

#include "inl/fractal_renderer.inl"
