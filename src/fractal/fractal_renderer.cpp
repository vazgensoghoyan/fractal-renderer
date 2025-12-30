#include "fractal/fractal_renderer.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay::fractal;
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

// public (yet) constructor

FractalRenderer::FractalRenderer(
    int width, int height,
    Viewport viewport,
    IterationFunc iterate,
    InitialFunc init,
    ParamFunc param,
    FractalConfig config
)
    : m_width(width)
    , m_height(height)
    , m_viewport(viewport)
    , m_iterate(iterate)
    , m_init(init)
    , m_param(param)
    , m_config(config)
{ }

Bmp FractalRenderer::render() {
    LOG_INFO("Starting fractal rendering: {}x{}, max_iter={}, escape_radius={:.2f}",
        m_width, m_height,
        m_config.max_iter, m_config.escape_radius
    );

    Bmp image = Bmp::empty(m_width, m_height);

    volatile double time_start = omp_get_wtime();

    const double real_min = m_viewport.min.real();
    const double real_max = m_viewport.max.real();
    const double imag_min = m_viewport.min.imag();
    const double imag_max = m_viewport.max.imag();

    const double real_step = (real_max - real_min) / (m_width  - 1);
    const double imag_step = (imag_max - imag_min) / (m_height - 1);

    const double escape_radius_sq = m_config.escape_radius * m_config.escape_radius;

    #pragma omp parallel for schedule(dynamic, 8)
    for (int y = 0; y < m_height; ++y) {

        #pragma omp simd
        for (int x = 0; x < m_width; ++x) {

            Complex pixel = pixel_to_complex_fast(
                x, y,
                real_min, imag_max,
                real_step, imag_step
            );

            Complex z = m_init(pixel);
            Complex c = m_param(pixel);

            int iter = 0;
            while (iter < m_config.max_iter) {
                const double zr = z.real();
                const double zi = z.imag();

                if (zr * zr + zi * zi > escape_radius_sq) {
                    break;
                }

                z = m_iterate(z, c);
                ++iter;
            }

            double mu = iter;

            if (iter < m_config.max_iter) {
                double zr = z.real();
                double zi = z.imag();
                double log_zn = std::log(zr * zr + zi * zi) / 2.0;
                double nu = std::log(log_zn / std::log(2.0)) / std::log(2.0);
                mu = iter + 1 - nu;
            }

            auto color_from_mu = [](double mu, int max_iter) -> BgrPixel {
                if (mu >= max_iter)
                    return {0, 0, 0};

                double t = mu / max_iter;

                uint8_t r = static_cast<uint8_t>(9 * (1 - t) * t * t * t * 255);
                uint8_t g = static_cast<uint8_t>(15 * (1 - t) * (1 - t) * t * t * 255);
                uint8_t b = static_cast<uint8_t>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

                return {b, g, r};
            };

            image.try_set_pixel(x, y, color_from_mu(mu, m_config.max_iter));
        }
    }

    volatile double time_end = omp_get_wtime();
    
    LOG_INFO("Fractal rendering completed in {:.3f} seconds", time_end - time_start);

    return image;
}
