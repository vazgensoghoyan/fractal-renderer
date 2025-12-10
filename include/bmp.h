#ifndef BMP_H_
#define BMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    uint8_t B;
    uint8_t G;
    uint8_t R;
} pixel_t;

typedef struct {
    uint16_t signature;       // 'BM' = 0x4D42
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t file_offset_to_pixels;
} bmp_fileheader_t; // 14 bytes

typedef struct {
    uint32_t header_size;     // must be 40 for BITMAPINFOHEADER
    int32_t  image_width;
    int32_t  image_height;    // if positive: bottom-up; if negative: top-down
    uint16_t planes;
    uint16_t bits_per_pixel;  // expect 24
    uint32_t compression;     // expect 0 (BI_RGB)
    uint32_t image_size;      // may be 0 for BI_RGB
    int32_t  x_pixels_per_meter;
    int32_t  y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t important_colors;
} bmp_infoheader_t; // 40 bytes

typedef struct {
    bmp_fileheader_t *fileheader;
    bmp_infoheader_t *infoheader;
    pixel_t **rows;    // pointers to each row, rows[0] = top row
} bmp_t;
#pragma pack(pop)

int load_bmp(const char *filepath, bmp_t *bitmap);
int crop(bmp_t *bmp, int x, int y, int width, int height);
int rotate(bmp_t *bmp);
int save_bmp(const char *filepath, bmp_t *bmp);
void free_bmp(bmp_t *bmp);

#endif