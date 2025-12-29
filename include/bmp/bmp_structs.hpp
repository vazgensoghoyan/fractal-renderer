#pragma once // bmp/bmp_structs.hpp

#include <cstdint>

namespace iheay::bmp {

#pragma pack(push, 1)

struct BmpFileHeader {
    char signature[2];        // 'B','M'
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixel_offset;
};

struct BmpInfoHeader {
    uint32_t header_size;     // must be 40
    int32_t width;
    int32_t height;           // >0 bottom-up, <0 top-down
    uint16_t planes;          // must be 1
    uint16_t bits_per_pixel;  // must be 24
    uint32_t compression;     // must be 0 (BI_RGB)
    uint32_t image_size;      // may be 0, but we fill it
    int32_t x_ppm;
    int32_t y_ppm;
    uint32_t colors_used;
    uint32_t colors_important;
};

struct BgrPixel {
    uint8_t b, g, r;
};

#pragma pack(pop)

} // namespace iheay::bmp
