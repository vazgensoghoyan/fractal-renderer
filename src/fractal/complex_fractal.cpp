#include "fractal/complex_fractal.hpp"

using namespace iheay;
using namespace iheay::math;
using namespace iheay::fractal;

static Complex pixel_to_complex(
    int x, int y,
    int w, int h,
    const Viewport& v
) {
    double real = v.min.get_real()
        + double(x) / (w - 1)
        * (v.max.get_real() - v.min.get_real());

    double imag = v.max.get_imag()
        - double(y) / (h - 1)
        * (v.max.get_imag() - v.min.get_imag());

    return Complex::Algebraic(real, imag);
}

void iheay::fractal::render_complex_fractal(
    raster::IPixeled& image,
    const Viewport& view,
    IterationFunc iterate,
    InitialFunc initial,
    ParamFunc parameter,
    const FractalConfig& cfg
) {
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {

            Complex pixel = pixel_to_complex(
                x, y,
                image.width(),
                image.height(),
                view
            );

            Complex z = initial(pixel);
            Complex c = parameter(pixel);

            int iter = 0;
            while (iter < cfg.max_iter &&
                   z.get_modulus() <= cfg.escape_radius) {
                z = iterate(z, c);
                ++iter;
            }

            uint8_t shade = (iter == cfg.max_iter)
                ? 0
                : static_cast<uint8_t>(255.0 * iter / cfg.max_iter);

            image.set_pixel(x, y, {shade, shade, shade});
        }
    }
}
