#pragma once // adapters/raylib_image_adapter.hpp

#include "raylib.h"

namespace iheay::adapters {

class RaylibImageAdapter {
public:
    using pixel_type = Color; // raylib color

    RaylibImageAdapter(Image& img) : m_image(img) {}

    int width() const { return m_image.width; }
    int height() const { return m_image.height; }

    void set_pixel(int x, int y, Color color) {
        ImageDrawPixel(&m_image, x, y, color);
    }

private:
    Image& m_image;
};

} // namespace iheay::adapters
