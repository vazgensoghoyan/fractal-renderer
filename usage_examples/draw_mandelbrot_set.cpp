#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"
#include "fractal/fractal_renderer_builder.hpp"
#include <omp.h>

using namespace iheay::math;
using namespace iheay::bmp;
using namespace iheay::fractal;

struct BgrColorizer {
    using pixel_type = BgrPixel;

    BgrPixel operator()(double mu, int max_iter) const {
        if (mu >= max_iter)
            return {0, 0, 0};

        double t = mu / max_iter;

        uint8_t r = static_cast<uint8_t>(9  * (1 - t) * t * t * t * 255);
        uint8_t g = static_cast<uint8_t>(15 * (1 - t) * (1 - t) * t * t * 255);
        uint8_t b = static_cast<uint8_t>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

        return {b, g, r};
    }
};

int main() {

    auto renderer = 
        FractalRendererBuilder<BgrColorizer>
            ::get_builder()
                .set_viewport_width(3)
                .set_viewport_center(-0.75)
                .set_iteration_func( [](auto& z, auto& c) { return z * z + c; } )
                .set_initial_func( [](auto&) { return Complex::Zero(); } )
                .set_param_func( [](auto& pixel) { return pixel; } )
                .build();

    Bmp image = Bmp::empty(1500, 1500);
    renderer.render(image);

    io::save(image, "mandelbrot_set.bmp");

    return 0;
}
