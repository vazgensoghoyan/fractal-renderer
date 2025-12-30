#include "fractal/abstract_fractal_renderer.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay::bmp;
using namespace iheay::math;
using namespace iheay::fractal;

Complex AbstractFractalRenderer::pixel_to_complex(int x, int y, double real_step, double imag_step) {
    return math::Complex::Algebraic(
        m_viewport.min.real() + x * real_step,
        m_viewport.max.imag() - y * imag_step
    );
}

Bmp AbstractFractalRenderer::render() {
    LOG_INFO("Starting fractal rendering: {}x{}, max_iter={}, escape_radius={:.2f}",
        m_width, m_height,
        m_config.max_iter, m_config.escape_radius
    );

    volatile double time_start = omp_get_wtime();

    Bmp image = Bmp::empty(m_width, m_height);

    const double real_step = (m_viewport.max.real() - m_viewport.min.real()) / (m_width  - 1);
    const double imag_step = (m_viewport.max.imag() - m_viewport.min.imag()) / (m_height - 1);

    const double escape_radius_sq = m_config.escape_radius * m_config.escape_radius;

    #pragma omp parallel for schedule(dynamic, 8)
    for (int y = 0; y < m_height; ++y) {

        #pragma omp simd
        for (int x = 0; x < m_width; ++x) {

            Complex pixel = pixel_to_complex(x, y, real_step, imag_step);

            Complex z = initialize_pixel(pixel);
            Complex c = get_param(pixel);

            int iter = 0;
            while (iter < m_config.max_iter) {
                const double zr = z.real();
                const double zi = z.imag();

                if (zr * zr + zi * zi > escape_radius_sq) {
                    break;
                }

                z = iterate_pixel(z, c);
                ++iter;
            }

            double mu = smooth(z, iter);

            image.try_set_pixel(x, y, colorize(mu));
        }
    }

    volatile double time_end = omp_get_wtime();
    
    LOG_INFO("Fractal rendering completed in {:.3f} seconds", time_end - time_start);

    return image;
}

double AbstractFractalRenderer::smooth(const Complex& z, int iter) {
    double mu = iter;

    if (iter < m_config.max_iter) {
        double zr = z.real();
        double zi = z.imag();
        double log_zn = std::log(zr * zr + zi * zi) / 2.0;
        double nu = std::log(log_zn / std::log(2.0)) / std::log(2.0);
        mu = iter + 1 - nu;
    }

    return mu;
}


BgrPixel AbstractFractalRenderer::colorize(double mu) {
    if (mu >= m_config.max_iter)
        return {0, 0, 0};

    double t = mu / m_config.max_iter;

    uint8_t r = static_cast<uint8_t>(9 * (1 - t) * t * t * t * 255);
    uint8_t g = static_cast<uint8_t>(15 * (1 - t) * (1 - t) * t * t * 255);
    uint8_t b = static_cast<uint8_t>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return {b, g, r};
}
