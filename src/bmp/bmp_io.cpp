#include "bmp/io/bmp_io.hpp"
#include "utils/logger.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <span>
#include <algorithm>
#include <cstring>

using namespace iheay::bmp;

// local static util

static constexpr int row_size_bytes(int width) noexcept {
    return ((width * 24 + 31) / 32) * 4;
}

// image loading

Bmp io::load(const std::string& path) {
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

    if (ih.width <= 0)
        throw std::runtime_error("Invalid BMP width");

    const bool bottom_up = ih.height > 0;
    const int width = ih.width;
    const int height = std::abs(ih.height);

    if ((int64_t)width * height > 1'000'000'000)
        std::runtime_error("BMP too large");

    std::vector<BgrPixel> pixels_buffer(width * height);

    const int row_size = row_size_bytes(width);
    std::vector<uint8_t> row(row_size);

    file.seekg(fh.pixel_offset);
    if (!file)
        throw std::runtime_error("Invalid pixel offset");

    for (int row_index = 0; row_index < height; ++row_index) {
        const int y = bottom_up ? (height - 1 - row_index) : row_index;

        if (!file.read(reinterpret_cast<char*>(row.data()), row_size))
            throw std::runtime_error("Unexpected EOF on file " + path);

        std::memcpy(
            &pixels_buffer[y * width],
            row.data(),
            width * sizeof(BgrPixel)
        );
    }

    LOG_INFO("Loaded BMP image: {} ({}x{})", path, width, height);

    return Bmp(width, height, pixels_buffer);
}

// image saving

void io::save(const Bmp& bmp, const std::string& path) {
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot create file: " + path);

    const int row_size = row_size_bytes(bmp.width());
    const int image_size = row_size * bmp.height();
    const int pixel_offset = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

    BmpFileHeader fh{
        {'B','M'},
        (uint32_t)(pixel_offset + image_size),
        0, 0,
        (uint32_t)(pixel_offset)
    };

    BmpInfoHeader ih{};
    ih.header_size    = 40;
    ih.width          = bmp.width();
    ih.height         = bmp.height(); // сохраняем bottom-up
    ih.planes         = 1;
    ih.bits_per_pixel = 24;
    ih.compression    = 0;
    ih.image_size     = (uint32_t)image_size;

    file.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    file.write(reinterpret_cast<const char*>(&ih), sizeof(ih));

    std::vector<uint8_t> row(row_size, 0);
    for (int y = bmp.height() - 1; y >= 0; --y) {
        std::memcpy(
            row.data(),
            &bmp.pixels()[y * bmp.width()],
            bmp.width() * sizeof(BgrPixel)
        );
        file.write(reinterpret_cast<const char*>(row.data()), row_size);
    }

    LOG_INFO("Saved BMP image: {} ({}x{})", path, bmp.width(), bmp.height());
}
