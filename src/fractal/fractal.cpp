#include "fractal/fractal.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay;
using namespace iheay::math;
using namespace iheay::bmp;

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

Bmp iheay::fractal::render_complex_fractal(
    int width, int height,
    const Viewport& view,
    IterationFunc iterate,
    InitialFunc initial,
    ParamFunc parameter,
    const FractalConfig& cfg
) {
    LOG_INFO("Starting fractal rendering: %dx%d, max_iter=%d, escape_radius=%.2f",
        width, height,
        cfg.max_iter, cfg.escape_radius
    );

    Bmp image = Bmp::empty(width, height);

    volatile double time_start = omp_get_wtime();

    const double real_min = view.min.real();
    const double real_max = view.max.real();
    const double imag_min = view.min.imag();
    const double imag_max = view.max.imag();

    const double real_step = (real_max - real_min) / (width  - 1);
    const double imag_step = (imag_max - imag_min) / (height - 1);

    const double escape_radius_sq = cfg.escape_radius * cfg.escape_radius;

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
                const double zr = z.real();
                const double zi = z.imag();

                if (zr * zr + zi * zi > escape_radius_sq) {
                    break;
                }

                z = iterate(z, c);
                ++iter;
            }

            double mu = iter;

            if (iter < cfg.max_iter) {
                double zr = z.real();
                double zi = z.imag();
                double log_zn = std::log(zr * zr + zi * zi) / 2.0;
                double nu = std::log(log_zn / std::log(2.0)) / std::log(2.0);
                mu = iter + 1 - nu;
            }

            auto color_from_mu = [](double mu, int max_iter) -> Pixel {
                if (mu >= max_iter)
                    return {0, 0, 0};

                double t = mu / max_iter;

                uint8_t r = static_cast<uint8_t>(9 * (1 - t) * t * t * t * 255);
                uint8_t g = static_cast<uint8_t>(15 * (1 - t) * (1 - t) * t * t * 255);
                uint8_t b = static_cast<uint8_t>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

                return {r, g, b};
            };

            image.try_set_pixel(x, y, color_from_mu(iter, cfg.max_iter));
        }
    }

    volatile double time_end = omp_get_wtime();
    
    LOG_INFO("Fractal rendering completed in %.3f seconds", time_end - time_start);

    return image;
}
