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

            const uint8_t shade =
                (iter == cfg.max_iter)
                    ? 0
                    : static_cast<uint8_t>(
                          255.0 * iter / cfg.max_iter
                      );

            image.set_pixel(x, y, {shade, shade, shade});
        }
    }
}
