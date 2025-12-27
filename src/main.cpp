#include <iostream>

#include "bmp/bmp.hpp"
#include "bmp/bmp_io.hpp"
#include "bmp/bmp_colors.hpp"
#include "rasterizer/rasterizer.hpp"
#include "fractal/complex_fractal.hpp"
#include "fractal/concrete_fractals.hpp"

using namespace iheay;

int main() {

    const int WIDTH = 2000;
    const int HEIGHT = 2000;

    bmp::Bmp img = bmp::Bmp::empty(WIDTH, HEIGHT);

    img = fractal::draw_mandelbrot(WIDTH, HEIGHT);
    bmp::BmpIO::save(img, "mandelbrot.bmp");

    img = fractal::draw_julia(WIDTH, HEIGHT);
    bmp::BmpIO::save(img, "julia.bmp");

    img = fractal::draw_burning_ship(WIDTH, HEIGHT);
    bmp::BmpIO::save(img, "burning_ship.bmp");

    return 0;
}
