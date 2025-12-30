#include "bmp/bmp.hpp"
#include <stdexcept>

using namespace iheay::bmp;

// default private constructor

Bmp::Bmp() = default;

Bmp::Bmp(int width, int height, const std::vector<BgrPixel>& pixels)
    : m_width(width)
    , m_height(height)
    , m_pixels(pixels)
{ 
    if (width <= 0 || height <= 0)
        throw std::runtime_error("Given width or height < 0 for bmp");
}

// little public fabric

Bmp Bmp::empty(int width, int height) {
    BgrPixel white_color{ 255, 255, 255 };
    return empty(width, height, white_color);
}

Bmp Bmp::empty(int width, int height, BgrPixel pixel) {
    std::vector<BgrPixel> pixels(width * height, pixel);
    return Bmp(width, height, pixels);
}

// width and height properties

int Bmp::width() const {
    return m_width;
}

int Bmp::height() const {
    return m_height;
}

// pixel property
    
void Bmp::set_pixel(int x, int y, BgrPixel pixel) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        throw std::runtime_error("Pixel out of bounds");

    m_pixels[y * m_width + x] = pixel;
}
    
bool Bmp::try_set_pixel(int x, int y, BgrPixel pixel) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;
    m_pixels[y * m_width + x] = pixel;
    return true;
}

BgrPixel Bmp::get_pixel(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        throw std::runtime_error("Pixel out of bounds");

    return m_pixels[y * m_width + x];
}
