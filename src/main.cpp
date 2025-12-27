#include <iostream>

#include "bmp/bmp.hpp"
#include "bmp/bmp_io.hpp"
#include "bmp/bmp_colors.hpp"
#include "rasterizer/rasterizer.hpp"
#include "fractal/complex_fractal.hpp"
#include "fractal/concrete_fractals.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay;

int main() {

    try {

        const int WIDTH = 8000;
        const int HEIGHT = 8000;

        bmp::Bmp img = bmp::Bmp::empty(WIDTH, HEIGHT);

        img = fractal::draw_mandelbrot(WIDTH, HEIGHT);
        bmp::BmpIO::save(img, "mandelbrot.bmp");

        img = fractal::draw_julia(WIDTH, HEIGHT);
        bmp::BmpIO::save(img, "julia.bmp");

    } catch (const std::exception& e) {
        
        LOG_ERROR("Exception: {}", e.what());

        return 1;
    }

    return 0;
}
