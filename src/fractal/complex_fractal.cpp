#include "fractal/complex_fractal.hpp"
#include <omp.h>

using namespace iheay;
using namespace iheay::math;

namespace {

inline Complex pixel_to_complex_fast(
    int x, int y,
    double real_min, double imag_max,
    double real_step, double imag_step
) {
    return Complex::Algebraic(
        real_min + x * real_step,
        imag_max - y * imag_step
    );
}

} // namespace

void iheay::fractal::render_complex_fractal(
    raster::IPixeled& image,
    const Viewport& view,
    IterationFunc iterate,
    InitialFunc initial,
    ParamFunc parameter,
    const FractalConfig& cfg
) {
    const int width  = image.width();
    const int height = image.height();

    const double real_min = view.min.get_real();
    const double real_max = view.max.get_real();
    const double imag_min = view.min.get_imag();
    const double imag_max = view.max.get_imag();

    const double real_step = (real_max - real_min) / (width  - 1);
    const double imag_step = (imag_max - imag_min) / (height - 1);

    const double escape_radius_sq =
        cfg.escape_radius * cfg.escape_radius;

    #pragma omp parallel for schedule(dynamic, 8)
    for (int y = 0; y < height; ++y) {

        #pragma omp simd
        for (int x = 0; x < width; ++x) {

            Complex pixel = pixel_to_complex_fast(
                x, y,
                real_min, imag_max,
                real_step, imag_step
            );

            Complex z = initial(pixel);
            Complex c = parameter(pixel);

            int iter = 0;
            while (iter < cfg.max_iter) {
                const double zr = z.get_real();
                const double zi = z.get_imag();

                if (zr * zr + zi * zi > escape_radius_sq) {
                    break;
                }

                z = iterate(z, c);
                ++iter;
            }

            auto color_from_iter = [](int iter, int max_iter) -> iheay::bmp::Pixel {
                if (iter == max_iter)
                    return {0, 0, 0};

                uint8_t r = static_cast<uint8_t>(9 * (1 - iter / (double)max_iter) * pow(iter / (double)max_iter, 3) * 255);
                uint8_t g = static_cast<uint8_t>(15 * pow(1 - iter / (double)max_iter, 2) * pow(iter / (double)max_iter, 2) * 255);
                uint8_t b = static_cast<uint8_t>(8.5 * pow(1 - iter / (double)max_iter, 3) * (iter / (double)max_iter) * 255);

                return {b, g, r};
            };

            image.set_pixel(x, y, color_from_iter(iter, cfg.max_iter));
        }
    }
}
