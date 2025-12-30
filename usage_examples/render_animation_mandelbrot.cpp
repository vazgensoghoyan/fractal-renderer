#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"
#include "fractal/fractal_renderer.hpp"
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
    const int FRAMES = 300;

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

    for (int i = 0; i < 1; ++i) {
        double t = static_cast<double>(i) / (FRAMES - 1);

        auto key = interpolate(start, end, t);
        auto view = make_viewport(key.camera, WIDTH, HEIGHT);

        LOG_INFO("Rendering frame {} / {}", i, FRAMES);

        FractalRenderer renderer = FractalRenderer(
            WIDTH, HEIGHT,
            view,
            [](auto& z, auto& c) { return z * z + c; },
            [](auto&) { return Complex::Zero(); },
            [](auto& pixel) { return pixel; },
            key.config
        );

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
