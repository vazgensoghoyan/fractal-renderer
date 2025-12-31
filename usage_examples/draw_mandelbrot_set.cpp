#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"
#include "fractal/fractal_renderer_builder.hpp"
#include <omp.h>

using namespace iheay::math;
using namespace iheay::bmp;
using namespace iheay::fractal;

int main() {

    auto renderer =
        FractalRendererBuilder
            ::get_builder()
                .set_image_width(2000)
                .set_image_height(2000)
                .set_initial_func( [](auto&) { return Complex::Zero(); } )
                .set_param_func( [](auto& pixel) { return pixel; } )
                .build();

    Bmp image = renderer.render();

    io::save(image, "mandelbrot_set.bmp");

    return 0;
}
