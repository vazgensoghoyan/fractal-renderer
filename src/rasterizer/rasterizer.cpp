#include "rasterizer/rasterizer.hpp"

using namespace iheay::bmp;

void iheay::raster::draw_line_dda(IPixeled& image, int x0, int y0, int x1, int y1, Pixel color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = std::max(abs(dx), abs(dy));
    if (steps == 0) {
        image.set_pixel(x0, y0, color);
        return;
    }

    float x_inc = dx / float(steps);
    float y_inc = dy / float(steps);

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; ++i) {
        image.set_pixel(int(x + 0.5f), int(y + 0.5f), color);
        x += x_inc;
        y += y_inc;
    }
}

void iheay::raster::draw_line_bresenham(IPixeled& image, int x0, int y0, int x1, int y1, Pixel color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        image.set_pixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

void iheay::raster::fill_background(IPixeled& image, iheay::bmp::Pixel color) {
    for (int y = 0; y < image.get_height(); ++y)
        for (int x = 0; x < image.get_width(); ++x)
            image.set_pixel(x, y, color);
}
