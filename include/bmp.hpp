#pragma once

#include "bmp_structs.hpp"
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstring>

class Bmp {
public:
    static Bmp empty(int width, int height); // empty white image
    static Bmp empty(int width, int height, const Pixel& pixel); // empty image of given color
    static Bmp load(const std::string& path);

    void save(const std::string& path) const;

    int get_width() const;
    int get_height() const;
    
    void set_pixel(int x, int y, const Pixel& pixel);
    bool set_pixel_unsafe(int x, int y, const Pixel& pixel); // true if set
    const Pixel& get_pixel(int x, int y) const;

private:
    Bmp();
    Bmp(int width, int height, const std::vector<Pixel> pixels);

    static uint32_t row_size_bytes(int width);

private:
    int m_width  = 0;
    int m_height = 0; // always positive
    std::vector<Pixel> m_pixels;

};
