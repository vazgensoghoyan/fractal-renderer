#pragma once // bmp.hpp

#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstring>

#include "bmp_structs.hpp"
#include "rasterizer/interface_pixeled.hpp"

namespace iheay::bmp {

    class Bmp : public iheay::raster::IPixeled {
    
    public:
        static Bmp empty(int width, int height); // empty white image
        static Bmp empty(int width, int height, Pixel pixel); // empty image of given color
        static Bmp load(const std::string& path);

        void save(const std::string& path) const;

        int get_width() const override;
        int get_height() const override;
        
        void set_pixel(int x, int y, Pixel pixel) override;
        bool try_set_pixel(int x, int y, Pixel pixel) override; // true if set
        Pixel get_pixel(int x, int y) const override;

    private:
        Bmp();
        Bmp(int width, int height, const std::vector<Pixel>& pixels);

        static uint32_t row_size_bytes(int width);

    private:
        int m_width  = 0;
        int m_height = 0; // always positive
        std::vector<Pixel> m_pixels;

    };

}