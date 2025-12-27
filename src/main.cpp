#include <iostream>

#include "bmp/bmp.hpp"
#include "bmp/bmp_io.hpp"
#include "bmp/bmp_colors.hpp"
#include "rasterizer/rasterizer.hpp"
#include "fractal/complex_fractal.hpp"
#include "fractal/concrete_fractals.hpp"
#include <omp.h>

using namespace iheay;

int main() {

    const int WIDTH = 2000;
    const int HEIGHT = 2000;

    bmp::Bmp img = bmp::Bmp::empty(WIDTH, HEIGHT);

    volatile double start_time = omp_get_wtime();

    img = fractal::draw_mandelbrot(WIDTH, HEIGHT);
    bmp::BmpIO::save(img, "mandelbrot.bmp");

    img = fractal::draw_julia(WIDTH, HEIGHT);
    bmp::BmpIO::save(img, "julia.bmp");

    img = fractal::draw_burning_ship(WIDTH, HEIGHT);
    bmp::BmpIO::save(img, "burning_ship.bmp");

    volatile double end_time = omp_get_wtime();
    
    std::cout << "Total time: " << (end_time - start_time) << " seconds\n";

    return 0;
}
