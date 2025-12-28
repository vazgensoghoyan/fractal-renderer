#pragma once // fractal/fractal_concretes.hpp

#include "bmp/bmp.hpp"
#include "math/complex.hpp"

namespace iheay::fractal {

iheay::bmp::Bmp draw_mandelbrot(int width, int height);

iheay::bmp::Bmp draw_julia(
    int width, int height, 
    math::Complex k = math::Complex::Algebraic(-0.8, 0.156)
);

} // namespace iheay::fractal
