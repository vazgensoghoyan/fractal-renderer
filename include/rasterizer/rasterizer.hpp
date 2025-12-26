#pragma once // rasterizer.hpp

#include <concepts>

#include "interface_pixeled.hpp"
#include "bmp/bmp.hpp"
#include <algorithm>

namespace iheay::raster {

    void draw_line_dda(IPixeled& image, int x0, int y0, int x1, int y1, iheay::bmp::Pixel color);
    void draw_line_bresenham(IPixeled& image, int x0, int y0, int x1, int y1, iheay::bmp::Pixel color);
    void fill_background(IPixeled& image, iheay::bmp::Pixel color);

}
