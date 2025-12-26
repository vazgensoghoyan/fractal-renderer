#include "bmp/bmp.hpp"

using namespace iheay::bmp;

// private static util

uint32_t Bmp::row_size_bytes(int width) {
    return ((width * 24 + 31) / 32) * 4;
}

// default private constructor

Bmp::Bmp() = default;

Bmp::Bmp(int width, int height, const std::vector<Pixel>& pixels)
    : m_width(width)
    , m_height(height)
    , m_pixels(pixels)
{ }

// little public fabric

Bmp Bmp::empty(int width, int height) {
    Pixel white_color{ 255, 255, 255 };
    return empty(width, height, white_color);
}

Bmp Bmp::empty(int width, int height, Pixel pixel) {
    std::vector<Pixel> pixels(width * height, pixel);
    return Bmp(width, height, pixels);
}

Bmp Bmp::load(const std::string& path) {
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

// width and height properties

int Bmp::get_width() const {
    return m_width;
}

int Bmp::get_height() const {
    return m_height;
}

// pixel property
    
void Bmp::set_pixel(int x, int y, Pixel pixel) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        throw std::out_of_range("Pixel out of bounds");

    m_pixels[y * m_width + x] = pixel;
}
    
bool Bmp::try_set_pixel(int x, int y, Pixel pixel) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;
    m_pixels[y * m_width + x] = pixel;
    return true;
}

Pixel Bmp::get_pixel(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        throw std::out_of_range("Pixel out of bounds");

    return m_pixels[y * m_width + x];
}

// image saving

void Bmp::save(const std::string& path) const {
    std::ofstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot create file");

    const uint32_t row_size = row_size_bytes(m_width);
    const uint32_t image_size = row_size * m_height;
    const uint32_t pixel_offset = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

    BmpFileHeader fh{
        {'B','M'},
        pixel_offset + image_size,
        0, 0,
        pixel_offset
    };

    BmpInfoHeader ih{};
    ih.header_size     = 40;
    ih.width           = m_width;
    ih.height          = m_height; // bottom-up
    ih.planes          = 1;
    ih.bits_per_pixel  = 24;
    ih.compression     = 0;
    ih.image_size      = image_size;

    file.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    file.write(reinterpret_cast<const char*>(&ih), sizeof(ih));

    std::vector<uint8_t> row(row_size, 0);

    for (int y = m_height - 1; y >= 0; --y) {
        std::memcpy(
            row.data(),
            &m_pixels[y * m_width],
            m_width * sizeof(Pixel)
        );
        file.write(reinterpret_cast<const char*>(row.data()), row_size);
    }
}
