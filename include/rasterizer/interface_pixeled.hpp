#pragma once // rasterizer/interface_pixeled.hpp

#include "bmp/bmp_structs.hpp"

namespace iheay::raster {

class IPixeled {
public:
    virtual int width() const = 0;
    virtual int height() const = 0;
    
    virtual iheay::bmp::BgrPixel get_pixel(int x, int y) const = 0;
    
    virtual void set_pixel(int x, int y, iheay::bmp::BgrPixel pixel) = 0;
    virtual bool try_set_pixel(int x, int y, iheay::bmp::BgrPixel pixel) = 0; // true if set
};

} // namespace iheay::raster
