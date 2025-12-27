#pragma once // bmp/bmp.hpp

#include <vector>

#include "bmp/bmp_structs.hpp"
#include "rasterizer/interface_pixeled.hpp"

namespace iheay::bmp {

class Bmp : public iheay::raster::IPixeled {
public:
    static Bmp empty(int width, int height); // empty white image
    static Bmp empty(int width, int height, Pixel pixel); // empty image of given color

    int width() const override;
    int height() const override;

    Pixel get_pixel(int x, int y) const override;

    void set_pixel(int x, int y, Pixel pixel) override;
    bool try_set_pixel(int x, int y, Pixel pixel) override;

private:
    friend class BmpIO;

    Bmp();
    Bmp(int width, int height, const std::vector<Pixel>& pixels);

private:
    int m_width = 0;
    int m_height = 0; // always positive
    std::vector<Pixel> m_pixels;
};

} // namespace iheay::bmp
