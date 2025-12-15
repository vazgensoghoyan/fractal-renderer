#pragma once

#include "bmp_structs.hpp"

#include <cstdint>
#include <vector>
#include <string>


class Bmp {
public:
    static Bmp empty();
    static Bmp load(const std::string& path);
    
    void save(const std::string& path) const;

    int get_width() const;
    int get_height() const;

    void set_pixel(int x, int y, const Pixel& color);
    const Pixel& get_pixel(int x, int y) const;

    void set_pixel();

private:
    Bmp(const BmpFileHeader& file_h, const BmpV5Header& v5_h, const std::vector<Pixel>& p);

    static int padding(int width);

private:
    static constexpr int c_ALIGNMENT = 4;

    BmpFileHeader m_file_header;
    BmpV5Header m_v5_header;

    std::vector<Pixel> m_pixels;

};
