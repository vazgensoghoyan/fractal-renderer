#ifndef BMP_H
#define BMP_H

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

// constructors
int init_empty_bmp(bmp_t *bitmap, int width, int height);
int load_bmp(const char *filepath, bmp_t *bitmap);
// desctructor
void free_bmp(bmp_t *bmp);
// working with pixels
int set_pixel(bmp_t *bmp, int i, int j, pixel_t color);
int set_pixel_rgb(bmp_t *bmp, int i, int j, uint8_t r, uint8_t g, uint8_t b);
int get_pixel(bmp_t *bmp, int i, int j, pixel_t *out_color);
// saving to bmp
int save_bmp(const char *filepath, bmp_t *bmp);
int save_pixels_to_bmp(const char *filepath, pixel_t **rows, int width, int height);

#endif // BMP_H
