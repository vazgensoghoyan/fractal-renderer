#pragma once // bmp/bmp.hpp

#include "bmp/bmp_structs.hpp"
#include "rasterizer/interface_pixeled.hpp"
#include <vector>

namespace iheay::bmp {

class Bmp : public iheay::raster::IPixeled<BgrPixel> {
public:
    Bmp() = delete;
    Bmp(int width, int height, const std::vector<BgrPixel>& pixels);

    static Bmp empty(int width, int height); // empty white image
    static Bmp empty(int width, int height, BgrPixel pixel); // empty image of given color

    int width() const override;
    int height() const override;

    const std::vector<BgrPixel>& pixels() const;

    const BgrPixel& get_pixel(int x, int y) const override;

    void set_pixel(int x, int y, BgrPixel pixel) override;
    bool try_set_pixel(int x, int y, BgrPixel pixel) override;

private:
    int m_width = 0;
    int m_height = 0; // always positive
    std::vector<BgrPixel> m_pixels;
};

} // namespace iheay::bmp
