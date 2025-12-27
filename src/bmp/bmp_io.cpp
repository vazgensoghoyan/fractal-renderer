#include "bmp/bmp_io.hpp"

using namespace iheay::bmp;

// private static util

uint32_t BmpIO::row_size_bytes(int width) {
    return ((width * 24 + 31) / 32) * 4;
}

// image loading

Bmp BmpIO::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open file");

    BmpFileHeader fh{};
    BmpInfoHeader ih{};

    file.read(reinterpret_cast<char*>(&fh), sizeof(fh));
    file.read(reinterpret_cast<char*>(&ih), sizeof(ih));

    if (fh.signature[0] != 'B' || fh.signature[1] != 'M')
        throw std::runtime_error("Not a BMP file");

    if (ih.header_size != 40)
        throw std::runtime_error("Only BITMAPINFOHEADER supported");

    if (ih.planes != 1 || ih.bits_per_pixel != 24)
        throw std::runtime_error("Only 24bpp BMP supported");

    if (ih.compression != 0)
        throw std::runtime_error("Compressed BMP not supported");

    const bool bottom_up = ih.height > 0;
    const int width  = ih.width;
    const int height = std::abs(ih.height);

    Bmp bmp;
    bmp.m_width  = width;
    bmp.m_height = height;
    bmp.m_pixels.resize(width * height);

    const uint32_t row_size = row_size_bytes(width);
    std::vector<uint8_t> row(row_size);

    file.seekg(fh.pixel_offset);

    for (int row_index = 0; row_index < height; ++row_index) {
        const int y = bottom_up ? (height - 1 - row_index) : row_index;

        file.read(reinterpret_cast<char*>(row.data()), row_size);

        std::memcpy(
            &bmp.m_pixels[y * width],
            row.data(),
            width * sizeof(Pixel)
        );
    }

    return bmp;
}


// image saving

void BmpIO::save(const Bmp& bmp, const std::string& path) {
    std::ofstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot create file");

    const uint32_t row_size = row_size_bytes(bmp.m_width);
    const uint32_t image_size = row_size * bmp.m_height;
    const uint32_t pixel_offset = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

    BmpFileHeader fh{
        {'B','M'},
        pixel_offset + image_size,
        0, 0,
        pixel_offset
    };

    BmpInfoHeader ih{};
    ih.header_size     = 40;
    ih.width           = bmp.m_width;
    ih.height          = bmp.m_height; // bottom-up
    ih.planes          = 1;
    ih.bits_per_pixel  = 24;
    ih.compression     = 0;
    ih.image_size      = image_size;

    file.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    file.write(reinterpret_cast<const char*>(&ih), sizeof(ih));

    std::vector<uint8_t> row(row_size, 0);

    for (int y = bmp.m_height - 1; y >= 0; --y) {
        std::memcpy(
            row.data(),
            &bmp.m_pixels[y * bmp.m_width],
            bmp.m_width * sizeof(Pixel)
        );
        file.write(reinterpret_cast<const char*>(row.data()), row_size);
    }
}

