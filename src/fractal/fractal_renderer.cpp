#include "fractal/fractal_renderer.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay::math;
using namespace iheay::bmp;
using namespace iheay::fractal;

// constructor

FractalRenderer::FractalRenderer(
    int width, int height,
    double viewport_width, 
    math::Complex viewport_min,
    int max_iter,
    double escape_radius,
    IterationFunc iterate,
    InitialFunc init,
    ParamFunc param
)
: m_width(width), m_height(height)
, m_viewport_width(viewport_width)
, m_viewport_height(viewport_width * height / width)
, m_viewport_min(viewport_min)
, m_max_iter(max_iter), m_escape_radius(escape_radius)
, m_iterate(iterate), m_init(init), m_param(param) {}

// local static helpers

static inline Complex pixel_to_complex_fast(
    int x, int y,
    double real_min, double imag_max,
    double real_step, double imag_step
) {
    return Complex::Algebraic(
        real_min + x * real_step,
        imag_max - y * imag_step
    );
}

static double calc_mu(Complex z, double iter, int max_iter) {
    double mu = iter;

    if (iter < max_iter) {
        double zr = z.real();
        double zi = z.imag();
        double log_zn = std::log(zr * zr + zi * zi) / 2.0;
        double nu = std::log(log_zn / std::log(2.0)) / std::log(2.0);
        mu = iter + 1 - nu;
    }

    return mu;
}

static BgrPixel colorize(double mu, int max_iter) {
    if (mu >= max_iter)
        return {0, 0, 0};

    double t = mu / max_iter;

    uint8_t r = static_cast<uint8_t>(9 * (1 - t) * t * t * t * 255);
    uint8_t g = static_cast<uint8_t>(15 * (1 - t) * (1 - t) * t * t * 255);
    uint8_t b = static_cast<uint8_t>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return {b, g, r};
}

// rendering

Bmp FractalRenderer::render() const {
    LOG_INFO("Starting fractal rendering: {}x{}, max_iter={}, escape_radius={:.2f}",
        m_width, m_height,
        m_max_iter, m_escape_radius
    );

    Bmp image = Bmp::empty(m_width, m_height);

    volatile double time_start = omp_get_wtime();

    const double real_min = m_viewport_min.real();
    const double real_max = real_min + m_viewport_width;
    const double imag_min = m_viewport_min.imag();
    const double imag_max = imag_min + m_viewport_height;

    const double real_step = (real_max - real_min) / (m_width  - 1);
    const double imag_step = (imag_max - imag_min) / (m_height - 1);

    const double escape_radius_sq = m_escape_radius * m_escape_radius;

    #pragma omp parallel for collapse(2) schedule(dynamic, 64)
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {

            Complex pixel = pixel_to_complex_fast(x, y, real_min, imag_max, real_step, imag_step);

            Complex z = m_init(pixel);
            Complex c = m_param(pixel);

            int iter = 0;
            while (iter < m_max_iter) {
                const double zr = z.real();
                const double zi = z.imag();

                if (zr * zr + zi * zi > escape_radius_sq) {
                    break;
                }

                z = m_iterate(z, c);
                ++iter;
            }

            double mu = calc_mu(z, iter, m_max_iter);

            image.try_set_pixel(x, y, colorize(mu, m_max_iter));
        }
    }

    volatile double time_end = omp_get_wtime();
    
    LOG_INFO("Fractal rendering completed in {:.3f} seconds", time_end - time_start);

    return image;
}
