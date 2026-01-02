// fractal/inl/fractal_renderer.inl

#include "utils/logger.hpp"
#include <omp.h>

namespace iheay::fractal {

// constructor

template <ColorizerConcept Colorizer>
FractalRenderer<Colorizer>::FractalRenderer(
    FractalConfig config,
    Viewport viewport,
    IterationFunc iterate,
    InitialFunc init,
    ParamFunc param,
    Colorizer colorizer
)
: m_config(config)
, m_viewport(viewport)
, m_iterate(iterate)
, m_init(init)
, m_param(param)
, m_colorizer(colorizer) {}

// local static helpers

static inline math::Complex pixel_to_complex(
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
        m_config.max_iter, m_config.escape_radius
    );

    volatile double time_start = omp_get_wtime();

    const double viewport_height = m_viewport.width * image.height() / image.width();

    const double vp_min_real = m_viewport.center.real() - m_viewport.width / 2;
    const double vp_max_imag = m_viewport.center.imag() + viewport_height / 2;

    const double real_step = m_viewport.width / (image.width()  - 1);
    const double imag_step = viewport_height / (image.height() - 1);

    const double escape_radius_sq = m_config.escape_radius * m_config.escape_radius;

    #pragma omp parallel for collapse(2) schedule(static)
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {

            math::Complex pixel = pixel_to_complex(x, y, vp_min_real, vp_max_imag, real_step, imag_step);

            math::Complex z = m_init(pixel);
            math::Complex c = m_param(pixel);

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

            double mu = calc_mu(z, iter, m_config.max_iter);

            image.set_pixel(x, y, m_colorizer(mu, m_config.max_iter));
        }
    }

    volatile double time_end = omp_get_wtime();
    
    LOG_INFO("Fractal rendering completed in {:.3f} seconds", time_end - time_start);
}

} // namespace iheay::fractal
