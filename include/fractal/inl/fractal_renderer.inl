// fractal/inl/fractal_renderer.inl

#include "utils/logger.hpp"
#include <omp.h>

namespace iheay::fractal {

// constructor

template <ColorizerConcept Colorizer>
FractalRenderer<Colorizer>::FractalRenderer(
    double viewport_width, 
    math::Complex viewport_min,
    int max_iter,
    double escape_radius,
    IterationFunc iterate,
    InitialFunc init,
    ParamFunc param,
    Colorizer colorizer
)
: m_viewport_width(viewport_width)
, m_viewport_min(viewport_min)
, m_max_iter(max_iter), m_escape_radius(escape_radius)
, m_iterate(iterate), m_init(init)
, m_param(param), m_colorizer(colorizer) {}

// local static helpers

static inline math::Complex pixel_to_complex_fast(
    int x, int y,
    double real_min, double imag_max,
    double real_step, double imag_step
) {
    return math::Complex::Algebraic(
        real_min + x * real_step,
        imag_max - y * imag_step
    );
}

static double calc_mu(math::Complex z, double iter, int max_iter) {
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

// rendering

template <ColorizerConcept Colorizer>
template <raster::PixeledImage Image>
void FractalRenderer<Colorizer>::render(Image& image) const {
    LOG_INFO("Starting fractal rendering: {}x{}, max_iter={}, escape_radius={:.2f}",
        image.width(), image.height(),
        m_max_iter, m_escape_radius
    );

    volatile double time_start = omp_get_wtime();

    const double viewport_height = m_viewport_width * image.height() / image.width();

    const double real_min = m_viewport_min.real();
    const double real_max = real_min + m_viewport_width;
    const double imag_min = m_viewport_min.imag();
    const double imag_max = imag_min + viewport_height;

    const double real_step = (real_max - real_min) / (image.width()  - 1);
    const double imag_step = (imag_max - imag_min) / (image.height() - 1);

    const double escape_radius_sq = m_escape_radius * m_escape_radius;

    #pragma omp parallel for collapse(2) schedule(static)
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {

            math::Complex pixel = pixel_to_complex_fast(x, y, real_min, imag_max, real_step, imag_step);

            math::Complex z = m_init(pixel);
            math::Complex c = m_param(pixel);

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

            image.set_pixel(x, y, m_colorizer(mu, m_max_iter));
        }
    }

    volatile double time_end = omp_get_wtime();
    
    LOG_INFO("Fractal rendering completed in {:.3f} seconds", time_end - time_start);
}

} // namespace iheay::fractal
