#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"
#include "fractal/fractal_renderer_builder.hpp"
#include "fractal/fractal_animation.hpp"
#include "utils/logger.hpp"
#include <omp.h>
#include <filesystem>
#include <string>

using namespace iheay::bmp;
using namespace iheay::math;
using namespace iheay::fractal;

namespace fs = std::filesystem;

bool directory_exists(std::string dir_name) {
    fs::path folder_path = dir_name;

    return fs::exists(folder_path) && fs::is_directory(folder_path);
}

std::string create_new_dir_name(std::string dir_name) {
    while (directory_exists(dir_name))
        dir_name += '_';
    return dir_name;
}

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

void render_animation() {

    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    const int FRAMES_COUNT = 300;

    std::string dir_name = create_new_dir_name("frames");
    fs::path directory_path = dir_name;
    fs::create_directory(directory_path);

    volatile double time_start = omp_get_wtime();

    Bmp image = Bmp::empty(WIDTH, HEIGHT);

    FractalKeyframe start {
        { 5, Complex::Algebraic(-0.75, 0.0) },
        { 300, 2.0 },
        Complex::Algebraic(-0.8, 0.156)
    };

    FractalKeyframe end {
        { 0.001, Complex::Algebraic(-0.74364388703, 0.13182590421) },
        { 2000, 2.0 },
        Complex::Algebraic(-0.8, 0.156)
    };

    auto renderer_builder = 
        FractalRendererBuilder<BgrColorizer>
            ::get_builder()
                .set_initial_func( [](auto&) { return Complex::Zero(); } )
                .set_param_func( [](auto& pixel) { return pixel; } );

    for (int i = 0; i < FRAMES_COUNT; ++i) {
        double t = static_cast<double>(i) / (FRAMES_COUNT - 1);

        auto key = interpolate(start, end, t);

        LOG_INFO("Rendering frame {} / {}", i, FRAMES_COUNT);

        auto renderer =
            renderer_builder
                .set_viewport(key.viewport)
                .build();

        renderer.render(image);

        std::string filename = std::format("{}/frame_{:04}.bmp", dir_name, i);

        io::save(image, filename);
    }

    volatile double time_end = omp_get_wtime();

    LOG_INFO("Animation rendering finished in {:.3f} seconds", time_end - time_start);
}

int main() {

    render_animation();

    return 0;
}
