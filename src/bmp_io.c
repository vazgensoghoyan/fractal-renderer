#include "bmp_io.h"
#include <stdio.h>
#include <stdlib.h>

static size_t row_byte_width(uint16_t bpp, int32_t w) {
    return (((size_t)bpp * (size_t)w + 31u) / 32u) * 4u;
}

int bmp_load(bmp_t *bmp, const char *path) {
    if (!bmp || !path) return 1;

    FILE *f = fopen(path, "rb");
    if (!f) return 1;

    bmp->fileheader = malloc(sizeof(bmp_fileheader_t));
    bmp->infoheader = malloc(sizeof(bmp_infoheader_t));
    bmp->rows = NULL;

    if (!bmp->fileheader || !bmp->infoheader) {
        fclose(f);
        free(bmp->fileheader);
        free(bmp->infoheader);
        return 1;
    }

    if (fread(bmp->fileheader, sizeof(bmp_fileheader_t), 1, f) != 1) goto fail;
    if (bmp->fileheader->signature != 0x4D42) goto fail;

    if (fread(bmp->infoheader, sizeof(bmp_infoheader_t), 1, f) != 1) goto fail;

    if (bmp->infoheader->bits_per_pixel != 24 ||
        bmp->infoheader->compression != 0 ||
        bmp->infoheader->header_size < 40)
        goto fail;

    int w = bmp->infoheader->image_width;
    int h = bmp->infoheader->image_height;
    int top_down = (h < 0);

    size_t H = top_down ? (size_t)(-h) : (size_t)h;
    size_t W = (size_t)w;

    size_t row_bytes = row_byte_width(24, w);
    size_t pixel_bytes = W * sizeof(pixel_t);
    size_t padding = row_bytes - pixel_bytes;

    pixel_t *buf = calloc(W * H, sizeof(pixel_t));
    pixel_t **rows = calloc(H, sizeof(pixel_t *));
    if (!buf || !rows) {
        free(buf); free(rows);
        goto fail;
    }

    for (size_t y = 0; y < H; y++) rows[y] = buf + y * W;

    fseek(f, bmp->fileheader->file_offset_to_pixels, SEEK_SET);

    for (size_t i = 0; i < H; i++) {
        size_t target = top_down ? i : (H - 1 - i);
        if (fread(rows[target], sizeof(pixel_t), W, f) != W) goto fail2;
        fseek(f, (long)padding, SEEK_CUR);
    }

    bmp->infoheader->image_height = (int32_t)H; /* normalize */
    bmp->rows = rows;
    fclose(f);
    return 0;

fail2:
    free(buf); free(rows);
fail:
    fclose(f);
    free(bmp->fileheader);
    free(bmp->infoheader);
    return 1;
}

int bmp_save(bmp_t *bmp, const char *path) {
    if (!bmp || !path || !bmp->rows || !bmp->fileheader || !bmp->infoheader)
        return 1;

    FILE *f = fopen(path, "wb");
    if (!f) return 1;

    int w = bmp->infoheader->image_width;
    int h = bmp->infoheader->image_height;

    size_t row_bytes = row_byte_width(24, w);
    size_t pixel_bytes = w * sizeof(pixel_t);
    size_t padding = row_bytes - pixel_bytes;

    bmp_fileheader_t fh = *bmp->fileheader;
    bmp_infoheader_t ih = *bmp->infoheader;

    ih.image_height = h;
    ih.image_size = (uint32_t)(row_bytes * h);

    fh.file_offset_to_pixels = sizeof(bmp_fileheader_t) + ih.header_size;
    fh.file_size = fh.file_offset_to_pixels + ih.image_size;

    fwrite(&fh, sizeof(fh), 1, f);
    fwrite(&ih, sizeof(ih), 1, f);

    uint8_t pad[4] = {0};

    for (int y = h - 1; y >= 0; y--) {
        fwrite(bmp->rows[y], sizeof(pixel_t), w, f);
        fwrite(pad, 1, padding, f);
    }

    fclose(f);
    return 0;
}
