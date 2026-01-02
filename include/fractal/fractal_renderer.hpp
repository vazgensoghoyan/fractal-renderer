#pragma once // fractal/fractal_renderer.hpp

#include "rasterizer/pixeled_concept.hpp"
#include "fractal/fractal_structures.hpp"
#include "math/complex.hpp"

namespace iheay::fractal {

template <ColorizerConcept Colorizer>
class FractalRenderer {
public:
    FractalRenderer(
        FractalConfig config,
        Viewport viewport,
        IterationFunc iterate,
        InitialFunc init,
        ParamFunc param,
        Colorizer colorizer
    );

    template <raster::PixeledImage Image>
    void render(Image& image) const;

private:
    FractalConfig m_config;
    Viewport m_viewport;
    IterationFunc m_iterate;
    InitialFunc m_init;
    ParamFunc m_param;
    Colorizer m_colorizer;
};
    
} // namespace iheay::fractal

#include "inl/fractal_renderer.inl"
