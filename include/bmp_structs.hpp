#ifndef BMP_STRUCTS_HPP
#define BMP_STRUCTS_HPP

#include <cstdint>

#pragma pack(push, 1)

struct Pixel {
    uint8_t b, g, r;
};

struct BmpFileHeader {
    char signature[2];
    uint32_t file_size;
    uint32_t reserved;
    uint32_t pixel_offset;
};

struct BmpV5Header {
    uint32_t header_size;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t image_size;
};

#pragma pack(pop)

#endif // BMP_STRUCTS_HPP
