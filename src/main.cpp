#include "bmp/bmp.hpp"
#include "bmp/bmp_io.hpp"

#include "fractal/fractal.hpp"
#include "fractal/fractal_camera.hpp"
#include "fractal/fractal_animation.hpp"

#include "math/complex.hpp"
#include "utils/logger.hpp"

#include <format>
#include <omp.h>

using namespace iheay;
using namespace iheay::math;
using namespace iheay::fractal;

int main() {
    try {
        const int WIDTH = 1920;
        const int HEIGHT = 1080;
        const int FRAMES = 300;

        bmp::Bmp image = bmp::Bmp::empty(WIDTH, HEIGHT);

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

        #pragma omp parallel for schedule(dynamic, 8)
        for (int i = 0; i < FRAMES; ++i) {
            double t = static_cast<double>(i) / (FRAMES - 1);

            auto key = interpolate(start, end, t);
            auto view = make_viewport(key.camera, WIDTH, HEIGHT);

            LOG_INFO("Rendering frame %d / %d", i + 1, FRAMES);

            auto image = render_complex_fractal(
                WIDTH, HEIGHT,
                view,
                [](auto& z, auto& c) { return z * z + c; },
                [](auto&) { return Complex::Zero(); },
                [](auto& pixel) { return pixel; },
                key.config
            );

            std::string filename = std::format("frames/frame_{:04}.bmp", i);

            bmp::BmpIO::save(image, filename);
        }

        LOG_INFO("Animation rendering finished");

    } catch (const std::exception& e) {
        LOG_ERROR("Exception: %s", e.what());
        return 1;
    }

    return 0;
}
