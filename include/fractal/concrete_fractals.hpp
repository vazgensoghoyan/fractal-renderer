#pragma once // concrete_fractals.hpp

#include "complex_fractal.hpp"
#include "bmp/bmp.hpp"
#include "bmp/bmp_io.hpp"
#include "math/complex.hpp"

namespace iheay::fractal {

    iheay::bmp::Bmp draw_mandelbrot(int width, int height);

    iheay::bmp::Bmp draw_julia(int width, int height);

    iheay::bmp::Bmp draw_burning_ship(int width, int height);

}
