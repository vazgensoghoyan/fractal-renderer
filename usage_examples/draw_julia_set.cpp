#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"
#include "math/complex.hpp"
#include "fractal/fractal_renderer.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay;
using namespace iheay::bmp;
using namespace iheay::math;

Bmp draw_julia(int width, int height, Complex k = Complex::Algebraic(-0.8, 0.156)) {
    LOG_INFO("Rendering Julia started");

    volatile double time_start = omp_get_wtime();

    fractal::Viewport view{
        Complex::Algebraic(-2.0, -1.5),
        Complex::Algebraic(1.0, 1.5)
    };

    fractal::FractalRenderer renderer = fractal::FractalRenderer(
        width, height,
        view,
        [](auto& z, auto& c) { return z*z + c; },
        [](auto& pixel) { return pixel; },
        [&](auto&) { return k; },
        {300, 2.0}
    );

    bmp::Bmp img = renderer.render();

    volatile double time_end = omp_get_wtime();

    LOG_INFO("Rendering Julia finished in {:.3f} seconds", time_end - time_start);

    return img;
}

int main() {

    Bmp image = draw_julia(3000, 3000);
    io::save(image, "julia_set.bmp");

    return 0;
}