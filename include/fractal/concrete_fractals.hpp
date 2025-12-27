#pragma once // concrete_fractals.hpp

#include "bmp/bmp.hpp"

namespace iheay::fractal {

iheay::bmp::Bmp draw_mandelbrot(int width, int height);
iheay::bmp::Bmp draw_julia(int width, int height);
iheay::bmp::Bmp draw_burning_ship(int width, int height);

} // namespace iheay::fractal
