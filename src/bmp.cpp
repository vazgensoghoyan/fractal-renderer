#include "bmp.hpp"

#include <stdlib.h>
#include <string.h>

static size_t row_byte_width(uint16_t bpp, int32_t w) {
    return (((size_t)bpp * (size_t)w + 31u) / 32u) * 4u;
}

int init_empty_bmp(bmp_t *bmp, int width, int height) {
    if (!bmp || width <= 0 || height <= 0)
        return 1;

    bmp->fileheader = new bmp_fileheader_t;
    bmp->infoheader = new bmp_infoheader_t;
    if (!bmp->fileheader || !bmp->infoheader)
        return 1;

    bmp_fileheader_t *fh = bmp->fileheader;
    bmp_infoheader_t *ih = bmp->infoheader;

    const int bpp = 24;
    size_t row_bytes = ((width * 3 + 3) / 4) * 4;
    size_t image_size = row_bytes * height;

    fh->signature = 0x4D42;
    fh->reserved1 = fh->reserved2 = 0;
    fh->file_offset_to_pixels = sizeof(bmp_fileheader_t) + sizeof(bmp_infoheader_t);
    fh->file_size = fh->file_offset_to_pixels + image_size;

    ih->header_size = 40;
    ih->image_width = width;
    ih->image_height = height;
    ih->planes = 1;
    ih->bits_per_pixel = bpp;
    ih->compression = 0;
    ih->image_size = image_size;
    ih->x_pixels_per_meter = 2835;
    ih->y_pixels_per_meter = 2835;
    ih->colors_used = 0;
    ih->important_colors = 0;

    pixel_t *buf = (pixel_t*)calloc((size_t)width * height, sizeof(pixel_t));
    pixel_t **rows = (pixel_t**)calloc(height, sizeof(pixel_t *));
    if (!buf || !rows) {
        free(buf); free(rows);
        return 1;
    }

    for (int y = 0; y < height; y++)
        rows[y] = buf + y * width;

    bmp->rows = rows;
    return 0;
}

int set_pixel(bmp_t *bmp, int x, int y, pixel_t p) {
    if (!bmp || !bmp->infoheader || !bmp->rows) return 1;

    int w = bmp->infoheader->image_width;
    int h = bmp->infoheader->image_height;

    if (h < 0) h = -h;

    if (x < 0 || y < 0 || x >= w || y >= h) return 1;

    bmp->rows[y][x] = p;
    return 0;
}

int set_pixel_rgb(bmp_t *bmp, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    pixel_t px = { b, g, r };
    return set_pixel(bmp, x, y, px);
}

int get_pixel(bmp_t *bmp, int x, int y, pixel_t *out) {
    if (!bmp || !bmp->infoheader || !bmp->rows || !out) return 1;

    int w = bmp->infoheader->image_width;
    int h = bmp->infoheader->image_height;

    if (h < 0) h = -h;
    if (x < 0 || y < 0 || x >= w || y >= h) return 1;

    *out = bmp->rows[y][x];
    return 0;
}

void free_bmp(bmp_t *bmp) {
    if (!bmp) return;

    if (bmp->rows) {
        if (bmp->rows[0]) free(bmp->rows[0]);
        free(bmp->rows);
    }
    free(bmp->fileheader);
    free(bmp->infoheader);

    bmp->fileheader = NULL;
    bmp->infoheader = NULL;
    bmp->rows = NULL;
}
