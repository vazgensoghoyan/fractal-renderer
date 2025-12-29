#pragma once // rasterizer/rasterizer.hpp

#include "rasterizer/interface_pixeled.hpp"
#include "bmp/bmp.hpp"

namespace iheay::raster {

void draw_line_dda(IPixeled& image, int x0, int y0, int x1, int y1, iheay::bmp::BgrPixel color);
void draw_line_bresenham(IPixeled& image, int x0, int y0, int x1, int y1, iheay::bmp::BgrPixel color);
void fill_background(IPixeled& image, iheay::bmp::BgrPixel color);

} // namespace iheay::raster
