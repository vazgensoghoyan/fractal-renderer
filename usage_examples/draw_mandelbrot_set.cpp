#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"
#include "math/complex.hpp"
#include "fractal/fractal_renderer.hpp"
#include "fractal/abstract_fractal_renderer.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay;
using namespace iheay::bmp;
using namespace iheay::math;
using namespace iheay::fractal;

class MandelbrotRenderer : public AbstractFractalRenderer {
public:
    MandelbrotRenderer(int width, int height, FractalConfig config, Viewport viewport) {
        m_width = width;
        m_height = height;
        m_config = config;
        m_viewport = viewport;
    }

private:
    math::Complex initialize_pixel(const math::Complex&) override {
        return Complex::Zero();
    }

    math::Complex get_param(const math::Complex& pixel) override {
        return pixel;
    }

    math::Complex iterate_pixel(math::Complex& z, const math::Complex& c) override {
        return z * z + c;
    }
};

int main() {

    auto renderer = MandelbrotRenderer(
        3000, 3000,
        {300, 2.0},
        Viewport{Complex::Algebraic(-2.0, -1.5), Complex::Algebraic(1.0, 1.5)}
    );

    Bmp image = renderer.render();

    io::save(image, "mandelbrot_set.bmp");

    return 0;
}
