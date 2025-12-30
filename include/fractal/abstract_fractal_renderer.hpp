#pragma once // fractal/abstract_fractal_renderer.hpp

#include "fractal/fractal_structures.hpp"
#include "math/complex.hpp"
#include "bmp/bmp.hpp"

namespace iheay::fractal {

// template pattern (I suppose it's called like that)

class AbstractFractalRenderer {
public:
    virtual ~AbstractFractalRenderer() = default;

    bmp::Bmp render();

protected:
    virtual math::Complex initialize_pixel(const math::Complex& pixel) = 0;
    virtual math::Complex get_param(const math::Complex& pixel) = 0;
    virtual math::Complex iterate_pixel(math::Complex& z, const math::Complex& c) = 0;
    
    virtual double smooth(const math::Complex& z, int iter);
    virtual bmp::BgrPixel colorize(double mu);

    math::Complex pixel_to_complex(int x, int y, double real_step, double imag_step);

protected:
    int m_width;
    int m_height;
    FractalConfig m_config;
    Viewport m_viewport;
};

} // namespace iheay::fractal
