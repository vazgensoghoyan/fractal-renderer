#include "fractal/fractal_concretes.hpp"
#include "fractal/fractal_renderer.hpp"
#include "bmp/bmp_io.hpp"
#include "math/complex.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay;
using namespace iheay::bmp;
using namespace iheay::math;

Bmp fractal::draw_mandelbrot(int width, int height) {
    LOG_INFO("Rendering Mandelbrot started");

    volatile double time_start = omp_get_wtime();

    fractal::Viewport view{
        Complex::Algebraic(-2.0, -1.5),
        Complex::Algebraic(1.0, 1.5)
    };

    fractal::FractalRenderer renderer = fractal::FractalRenderer(
        width, height,
        view,
        [](auto& z, auto& c) { return z*z + c; },
        [](auto&) { return Complex::Zero(); },
        [](auto& pixel) { return pixel; },
        {300, 2.0}
    );

    bmp::Bmp img = renderer.render();

    volatile double time_end = omp_get_wtime();

    LOG_INFO("Rendering Mandelbrot finished in {:.3f} seconds", time_end - time_start);

    return img;
}
