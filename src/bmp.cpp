#include "bmp.hpp"
#include <fstream>
#include <algorithm>

// static private util

int Bmp::padding(int width) {
    int rowBytes = sizeof(Pixel) * width;
    return (c_ALIGNMENT - rowBytes % c_ALIGNMENT) % c_ALIGNMENT;
}

// private constructor

Bmp::Bmp(const BmpFileHeader& file_h, const BmpV5Header& v5_h, const std::vector<Pixel>& p)
    : m_file_header(file_h)
    , m_v5_header(v5_h)
    , m_pixels(p)
{ }

// public little fabric



Bmp Bmp::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::ios_base::failure("Failed to open BMP file: " + path);

    BmpFileHeader file_header;
    BmpV5Header v5_header;
    std::vector<Pixel> pixels;

    file.read(reinterpret_cast<char*>(&file_header), sizeof(m_file_header));
    if (!file)
        throw std::runtime_error("Failed to read BMP file header");

    file.read(reinterpret_cast<char*>(&v5_header), sizeof(m_v5_header));
    if (!file)
        throw std::runtime_error("Failed to read BMP V5 header");

    file.seekg(file_header.pixel_offset);
    if (!file)
        throw std::runtime_error("Invalid BMP pixel offset");

    if (v5_header.width <= 0 || v5_header.height <= 0)
        throw std::runtime_error("Invalid BMP dimensions");

    pixels.resize(static_cast<size_t>(v5_header.width) * v5_header.height);

    int pad = padding(v5_header.width);

    for (int y = 0; y < v5_header.height; ++y) {
        file.read(reinterpret_cast<char*>(&pixels[y * v5_header.width]),
                  sizeof(Pixel) * v5_header.width);
        if (!file)
            throw std::runtime_error("Failed to read BMP pixel data");

        file.ignore(pad);
        if (!file)
            throw std::runtime_error("Failed to skip BMP padding");
    }

    return Bmp(file_header, v5_header, pixels);
}

// saving to file in path

void Bmp::save(const std::string& path) const {
    std::ofstream file(path, std::ios::binary);
    if (!file)
        throw std::ios_base::failure("Failed to create BMP file: " + path);

    file.write(reinterpret_cast<const char*>(&m_file_header), sizeof(m_file_header));
    if (!file)
        throw std::runtime_error("Failed to write BMP file header");

    file.write(reinterpret_cast<const char*>(&m_v5_header), sizeof(m_v5_header));
    if (!file)
        throw std::runtime_error("Failed to write BMP V5 header");

    file.seekp(m_file_header.pixel_offset);
    if (!file)
        throw std::runtime_error("Invalid BMP pixel offset");

    int pad = padding(get_width());
    uint32_t zero = 0;

    for (int y = 0; y < m_v5_header.height; ++y) {
        file.write(reinterpret_cast<const char*>(&m_pixels[y * m_v5_header.width]),
                   sizeof(Pixel) * m_v5_header.width);
        if (!file)
            throw std::runtime_error("Failed to write BMP pixel's row");

        file.write(reinterpret_cast<const char*>(&zero), pad);
        if (!file)
            throw std::runtime_error("Failed to write BMP padding");
    }
}
