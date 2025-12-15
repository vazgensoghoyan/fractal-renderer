#pragma once

#include "bmp/bmp_structs.hpp"

namespace iheay::raster {

    class IPixeled {

    public:
        virtual int get_width() const = 0;
        virtual int get_height() const = 0;
        
        virtual void set_pixel(int x, int y, Pixel pixel) = 0;
        virtual bool set_pixel_unsafe(int x, int y, Pixel pixel) = 0; // true if set
        virtual Pixel get_pixel(int x, int y) const = 0;

    };

}
