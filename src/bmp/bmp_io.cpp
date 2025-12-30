#include "bmp/bmp.hpp"
#include "bmp/bmp_io.hpp"
#include "utils/logger.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <span>
#include <algorithm>
#include <cstring>

using namespace iheay::bmp;

// local static util

static constexpr size_t row_size_bytes(size_t width) noexcept {
    return ((width * 24 + 31) / 32) * 4;
}

// image loading

Bmp BmpIO::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open file: " + path);

    BmpFileHeader fh{};
    BmpInfoHeader ih{};

    if (!file.read(reinterpret_cast<char*>(&fh), sizeof(fh)))
        throw std::runtime_error("Unexpected EOF reading file header: " + path);

    if (!file.read(reinterpret_cast<char*>(&ih), sizeof(ih)))
        throw std::runtime_error("Unexpected EOF reading info header: " + path);

    if (fh.signature[0] != 'B' || fh.signature[1] != 'M')
        throw std::runtime_error("Not a BMP file: " + path);
    if (ih.header_size != 40 || ih.planes != 1 || ih.bits_per_pixel != 24 || ih.compression != 0)
        throw std::runtime_error("Unsupported BMP format: " + path);

    const bool bottom_up = ih.height > 0;
    const size_t width = (size_t)ih.width;
    const size_t height = (size_t)std::abs(ih.height);

    Bmp bmp;
    bmp.m_width = (int)width;
    bmp.m_height = (int)height;
    bmp.m_pixels.resize(width * height);

    const size_t row_size = row_size_bytes(width);
    std::vector<uint8_t> buffer(row_size * height, 0);

    file.seekg(fh.pixel_offset);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), buffer.size()))
        throw std::runtime_error("Unexpected EOF reading pixel data: " + path);

    for (size_t row = 0; row < height; ++row) {
        const size_t y = bottom_up ? (height - 1 - row) : row;
        const uint8_t* row_ptr = buffer.data() + row * row_size;
        std::span<const BgrPixel> pixels(reinterpret_cast<const BgrPixel*>(row_ptr), width);
        std::copy_n(pixels.begin(), width, bmp.m_pixels.begin() + y * width);
    }

    LOG_INFO("Loaded BMP image: {} ({}x{})", path.c_str(), width, height);
    return bmp;
}

// image saving

void BmpIO::save(const Bmp& bmp, const std::string& path) {
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot create file: " + path);

    const size_t row_size = row_size_bytes(bmp.m_width);
    const size_t image_size = row_size * bmp.m_height;
    const size_t pixel_offset = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

    BmpFileHeader fh{
        {'B','M'},
        (uint32_t)(pixel_offset + image_size),
        0, 0,
        (uint32_t)(pixel_offset)
    };

    BmpInfoHeader ih{};
    ih.header_size    = 40;
    ih.width          = bmp.m_width;
    ih.height         = bmp.m_height; // сохраняем bottom-up
    ih.planes         = 1;
    ih.bits_per_pixel = 24;
    ih.compression    = 0;
    ih.image_size     = (uint32_t)image_size;

    file.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    file.write(reinterpret_cast<const char*>(&ih), sizeof(ih));

    std::vector<uint8_t> buffer(row_size * bmp.m_height, 0);

    for (size_t row = 0; row < static_cast<size_t>(bmp.m_height); ++row) {
        const size_t y = bmp.m_height - 1 - row; // bottom-up
        uint8_t* row_ptr = buffer.data() + row * row_size;
        std::span<BgrPixel> pixels(reinterpret_cast<BgrPixel*>(row_ptr), bmp.m_width);
        std::copy_n(bmp.m_pixels.begin() + y * bmp.m_width, bmp.m_width, pixels.begin());
    }

    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());

    LOG_INFO("Saved BMP image: {} ({}x{})", path.c_str(), bmp.m_width, bmp.m_height);
}
