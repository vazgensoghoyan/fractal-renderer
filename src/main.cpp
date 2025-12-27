#include <iostream>

#include "bmp/bmp.hpp"
#include "bmp/bmp_io.hpp"
#include "bmp/bmp_colors.hpp"
#include "rasterizer/rasterizer.hpp"

using namespace iheay;

void draw_image(bmp::Bmp& bmp) {
    int width  = bmp.get_width();
    int height = bmp.get_height();

    // заливка фона
    raster::fill_background(bmp, bmp::colors::WHITE);

    // диагонали через центр (DDA)
    raster::draw_line_dda(bmp, 0, 0, width - 1, height - 1, bmp::colors::RED);
    raster::draw_line_dda(bmp, 0, height - 1, width - 1, 0, bmp::colors::RED);

    // горизонтальная и вертикальная линии через центр (Bresenham)
    int cx = width / 2;
    int cy = height / 2;
    raster::draw_line_bresenham(bmp, 0, cy, width - 1, cy, bmp::colors::GREEN);  // горизонталь
    raster::draw_line_bresenham(bmp, cx, 0, cx, height - 1, bmp::colors::GREEN); // вертикаль

    // сетка каждые 50 пикселей (DDA)
    for (int i = 50; i < width; i += 50)
        raster::draw_line_dda(bmp, i, 0, i, height - 1, bmp::colors::BLUE);

    for (int j = 50; j < height; j += 50)
        raster::draw_line_dda(bmp, 0, j, width - 1, j, bmp::colors::BLUE);
}

void render(const std::string& output_file) {
    bmp::Bmp bmp = bmp::Bmp::empty(500, 700, bmp::colors::WHITE);

    draw_image(bmp);

    bmp::BmpIO::save(bmp, output_file);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " output_file.bmp\n";
        return 1;
    }

    try {
        render(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
