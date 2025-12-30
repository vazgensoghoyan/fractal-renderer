#pragma once // rasterizer/rasterizer.hpp

#include "rasterizer/interface_pixeled.hpp"

namespace iheay::raster {

template<HasRGB PixelType>
void draw_line_dda(IPixeled<PixelType>& image, int x0, int y0, int x1, int y1, PixelType color);

template<HasRGB PixelType>
void draw_line_bresenham(IPixeled<PixelType>& image, int x0, int y0, int x1, int y1, PixelType color);

template<HasRGB PixelType>
void fill_background(IPixeled<PixelType>& image, PixelType color);

} // namespace iheay::raster
