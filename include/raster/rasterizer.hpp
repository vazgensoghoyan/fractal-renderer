#pragma once

#include <concepts>

#include "interface_pixeled.hpp"

namespace iheay::raster {

    template <typename T>
    requires std::derived_from<IPixeled, T>
    class Rasterizer {

    public:
        void draw_line_dda(T& image, int x0, int y0, int x1, int y1, Pixel color);
        void draw_line_bresenham(T& image, int x0, int y0, int x1, int y1, Pixel color);

    };

}