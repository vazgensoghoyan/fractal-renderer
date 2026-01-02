#pragma once // rasterizer/rasterizer.hpp

#include "rasterizer/pixeled_concept.hpp"

namespace iheay::raster {

template <PixeledImage Image>
void draw_line_dda(Image& image, int x0, int y0, int x1, int y1, typename Image::pixel_type color);

template <PixeledImage Image>
void draw_line_bresenham(Image& image, int x0, int y0, int x1, int y1, typename Image::pixel_type color);

template <PixeledImage Image>
void fill_background(Image& image, typename Image::pixel_type color);

} // namespace iheay::raster
