#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"
#include "fractal/fractal_renderer_builder.hpp"
#include "fractal/fractal_animation.hpp"
#include "fractal/fractal_camera.hpp"
#include "utils/logger.hpp"
#include <omp.h>

using namespace iheay::bmp;
using namespace iheay::math;
using namespace iheay::fractal;

void render_animation() {

    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    const int FRAMES_COUNT = 300;

    volatile double time_start = omp_get_wtime();

    Bmp image = Bmp::empty(WIDTH, HEIGHT);

    FractalKeyframe start {
        { Complex::Algebraic(-0.75, 0.0), 2.5 },
        { 300, 2.0 },
        Complex::Algebraic(-0.8, 0.156)
    };

    FractalKeyframe end {
        { Complex::Algebraic(-0.74364388703, 0.13182590421), 0.0005 },
        { 2000, 2.0 },
        Complex::Algebraic(-0.8, 0.156)
    };

    auto renderer_builder = 
        FractalRendererBuilder
            ::get_builder()
                .set_image_width(WIDTH)
                .set_image_height(HEIGHT);

    for (int i = 0; i < FRAMES_COUNT; ++i) {
        double t = static_cast<double>(i) / (FRAMES_COUNT - 1);

        auto key = interpolate(start, end, t);
        auto viewport = make_viewport(key.camera, WIDTH, HEIGHT);

        LOG_INFO("Rendering frame {} / {}", i, FRAMES_COUNT);

        auto renderer =
            renderer_builder
                .set_viewport_min(viewport.min)
                .set_viewport_width(viewport.max.real() - viewport.min.real())
                .set_initial_func( [](auto&) { return Complex::Zero(); } )
                .set_param_func( [](auto& pixel) { return pixel; } )
                .build();

        Bmp image = renderer.render();

        std::string filename = std::format("frames/frame_{:04}.bmp", i);

        io::save(image, filename);
    }

    volatile double time_end = omp_get_wtime();

    LOG_INFO("Animation rendering finished in {:.3f} seconds", time_end - time_start);
}

int main() {

    render_animation();

    return 0;
}
