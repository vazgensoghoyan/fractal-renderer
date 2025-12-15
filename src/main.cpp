#include <iostream>

#include "bmp.hpp"
#include "bmp_colors.hpp"

using namespace iheay::bmp;

void draw_line_dda(Bmp& bmp, int x0, int y0, int x1, int y1, Pixel color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = std::max(abs(dx), abs(dy));
    if (steps == 0) {
        bmp.set_pixel(x0, y0, color);
        return;
    }

    float x_inc = dx / float(steps);
    float y_inc = dy / float(steps);

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; ++i) {
        bmp.set_pixel(int(x + 0.5f), int(y + 0.5f), color);
        x += x_inc;
        y += y_inc;
    }
}

void draw_line_bresenham(Bmp& bmp, int x0, int y0, int x1, int y1, Pixel color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        bmp.set_pixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

void fill_background(Bmp& bmp, Pixel color) {
    for (int y = 0; y < bmp.get_height(); ++y)
        for (int x = 0; x < bmp.get_width(); ++x)
            bmp.set_pixel(x, y, color);
}

void draw_image(Bmp& bmp) {
    int width  = bmp.get_width();
    int height = bmp.get_height();

    // заливка фона
    fill_background(bmp, colors::WHITE);

    // диагонали через центр (DDA)
    draw_line_dda(bmp, 0, 0, width - 1, height - 1, colors::RED);
    draw_line_dda(bmp, 0, height - 1, width - 1, 0, colors::RED);

    // горизонтальная и вертикальная линии через центр (Bresenham)
    int cx = width / 2;
    int cy = height / 2;
    draw_line_bresenham(bmp, 0, cy, width - 1, cy, colors::GREEN);  // горизонталь
    draw_line_bresenham(bmp, cx, 0, cx, height - 1, colors::GREEN); // вертикаль

    // сетка каждые 50 пикселей (DDA)
    for (int i = 50; i < width; i += 50)
        draw_line_dda(bmp, i, 0, i, height - 1, colors::BLUE);

    for (int j = 50; j < height; j += 50)
        draw_line_dda(bmp, 0, j, width - 1, j, colors::BLUE);
}

void render(const std::string& output_file) {
    Bmp bmp = Bmp::empty(500, 700, colors::WHITE);

    draw_image(bmp);

    bmp.save(output_file);
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
