#include "bmp.hpp"
#include <fstream>
#include <algorithm>

int Bmp::padding() const {
    int rowBytes = sizeof(Pixel) * v5Header.width;
    return (ALIGNMENT - rowBytes % ALIGNMENT) % ALIGNMENT;
}

const Pixel& Bmp::at(int x, int y) const {
    return pixels[y * v5Header.width + x];
}

void Bmp::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::ios_base::failure("Failed to open BMP file: " + path);

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (!file)
        throw std::runtime_error("Failed to read BMP file header");

    file.read(reinterpret_cast<char*>(&v5Header), sizeof(v5Header));
    if (!file)
        throw std::runtime_error("Failed to read BMP V5 header");

    file.seekg(fileHeader.pixel_offset);
    if (!file)
        throw std::runtime_error("Invalid BMP pixel offset");

    if (v5Header.width <= 0 || v5Header.height <= 0)
        throw std::runtime_error("Invalid BMP dimensions");

    pixels.resize(static_cast<size_t>(v5Header.width) * v5Header.height);

    int pad = padding();

    for (int y = 0; y < v5Header.height; ++y) {
        file.read(reinterpret_cast<char*>(&pixels[y * v5Header.width]),
                  sizeof(Pixel) * v5Header.width);
        if (!file)
            throw std::runtime_error("Failed to read BMP pixel data");

        file.ignore(pad);
        if (!file)
            throw std::runtime_error("Failed to skip BMP padding");
    }
}

void Bmp::save(const std::string& path) const {
    std::ofstream file(path, std::ios::binary);
    if (!file)
        throw std::ios_base::failure("Failed to create BMP file: " + path);

    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    if (!file)
        throw std::runtime_error("Failed to write BMP file header");

    file.write(reinterpret_cast<const char*>(&v5Header), sizeof(v5Header));
    if (!file)
        throw std::runtime_error("Failed to write BMP V5 header");

    file.seekp(fileHeader.pixel_offset);
    if (!file)
        throw std::runtime_error("Invalid BMP pixel offset");

    int pad = padding();
    uint32_t zero = 0;

    for (int y = 0; y < v5Header.height; ++y) {
        file.write(reinterpret_cast<const char*>(&pixels[y * v5Header.width]),
                   sizeof(Pixel) * v5Header.width);
        if (!file)
            throw std::runtime_error("Failed to write BMP pixel's row");

        file.write(reinterpret_cast<const char*>(&zero), pad);
        if (!file)
            throw std::runtime_error("Failed to write BMP padding");
    }
}
