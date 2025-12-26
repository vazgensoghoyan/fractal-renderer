#pragma once // interface_pixeled.hpp

#include "bmp/bmp_structs.hpp"

namespace iheay::raster {

    class IPixeled {

    public:
        [[nodiscard]] virtual int get_width() const = 0;
        [[nodiscard]] virtual int get_height() const = 0;
        
        [[nodiscard]] virtual iheay::bmp::Pixel get_pixel(int x, int y) const = 0;
        
        virtual void set_pixel(int x, int y, iheay::bmp::Pixel pixel) = 0;
        virtual bool try_set_pixel(int x, int y, iheay::bmp::Pixel pixel) = 0; // true if set

    };

}
