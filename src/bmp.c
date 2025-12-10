#include "bmp.h"

/* --- utilities --- */

static size_t row_byte_width(uint16_t bits_per_pixel, int32_t width) {
    // Row size in bytes rounded to 4 bytes: ((bits*width +31)/32)*4
    return (( (size_t)bits_per_pixel * (size_t)width + 31u) / 32u) * 4u;
}

static int safe_fread(void *ptr, size_t size, size_t nmemb, FILE *f) {
    return fread(ptr, size, nmemb, f) == nmemb ? 0 : 1;
}

/* --- core functions --- */

int load_bmp(const char *filepath, bmp_t *bmp) {
    if (!filepath || !bmp) return 1;
    FILE *f = fopen(filepath, "rb");
    if (!f) return 1;

    bmp->fileheader = malloc(sizeof(bmp_fileheader_t));
    bmp->infoheader = malloc(sizeof(bmp_infoheader_t));
    if (!bmp->fileheader || !bmp->infoheader) { fclose(f); return 1; }

    if (safe_fread(bmp->fileheader, sizeof(bmp_fileheader_t), 1, f)) goto fail;
    if (bmp->fileheader->signature != 0x4D42) goto fail; // 'BM'

    if (safe_fread(bmp->infoheader, sizeof(bmp_infoheader_t), 1, f)) goto fail;

    // Only support common uncompressed 24-bit BMP
    if (bmp->infoheader->header_size < 40) goto fail;
    if (bmp->infoheader->bits_per_pixel != 24) goto fail;
    if (bmp->infoheader->compression != 0) goto fail;

    int32_t width = bmp->infoheader->image_width;
    int32_t height = bmp->infoheader->image_height;
    if (width <= 0 || height == 0) goto fail;

    int top_down = (height < 0);
    size_t abs_height = (size_t)(top_down ? -height : height);
    size_t abs_width = (size_t)width;

    size_t row_bytes = row_byte_width(bmp->infoheader->bits_per_pixel, width);
    size_t pixel_row_bytes = abs_width * sizeof(pixel_t);
    size_t padding = row_bytes - pixel_row_bytes;

    // allocate contiguous pixel buffer and row pointers
    pixel_t *pixels = calloc(abs_width * abs_height, sizeof(pixel_t));
    pixel_t **rows = calloc(abs_height, sizeof(pixel_t *));
    if (!pixels || !rows) { free(pixels); free(rows); goto fail; }

    for (size_t r = 0; r < abs_height; ++r) {
        rows[r] = pixels + r * abs_width;
    }

    // go to pixel data
    if (fseek(f, bmp->fileheader->file_offset_to_pixels, SEEK_SET) != 0) { free(pixels); free(rows); goto fail; }

    // Read rows from file. BMP file order:
    // if bottom-up (height>0) — first row is bottom. We want rows[0] = top,
    // so map file row i -> rows[abs_height - 1 - i]
    for (size_t i = 0; i < abs_height; ++i) {
        size_t target_row = top_down ? i : (abs_height - 1 - i);
        if (fread(rows[target_row], sizeof(pixel_t), abs_width, f) != abs_width) { free(pixels); free(rows); goto fail; }
        if (padding) {
            if (fseek(f, (long)padding, SEEK_CUR) != 0) { free(pixels); free(rows); goto fail; }
        }
    }

    bmp->rows = rows;
    fclose(f);
    return 0;

fail:
    fclose(f);
    free(bmp->fileheader);
    free(bmp->infoheader);
    bmp->fileheader = bmp->infoheader = NULL;
    return 1;
}

int save_bmp(const char *filepath, bmp_t *bmp) {
    if (!filepath || !bmp || !bmp->rows || !bmp->fileheader || !bmp->infoheader) return 1;
    FILE *f = fopen(filepath, "wb");
    if (!f) return 1;

    int32_t width = bmp->infoheader->image_width;
    int32_t height = bmp->infoheader->image_height;
    if (width <= 0 || height <= 0) { fclose(f); return 1; }

    uint16_t bpp = bmp->infoheader->bits_per_pixel;
    size_t row_bytes = row_byte_width(bpp, width);
    size_t pixel_row_bytes = (size_t)width * sizeof(pixel_t);
    size_t padding = row_bytes - pixel_row_bytes;
    size_t image_size = row_bytes * (size_t)height;

    // prepare headers to write; we write a standard BITMAPINFOHEADER (40 bytes) and bottom-up image
    bmp_fileheader_t fh = *bmp->fileheader;
    bmp_infoheader_t ih = *bmp->infoheader;

    ih.image_height = height; // positive => bottom-up
    ih.image_size = (uint32_t)image_size;

    fh.file_offset_to_pixels = sizeof(bmp_fileheader_t) + ih.header_size;
    fh.file_size = fh.file_offset_to_pixels + (uint32_t)image_size;

    // write headers
    if (fwrite(&fh, sizeof(bmp_fileheader_t), 1, f) != 1) { fclose(f); return 1; }
    if (fwrite(&ih, sizeof(bmp_infoheader_t), 1, f) != 1) { fclose(f); return 1; }

    // pad buffer for writing padding bytes
    uint8_t padbuf[4] = {0,0,0,0};

    // write rows in bottom-up order: from last (bottom) to first (top)
    for (int r = height - 1; r >= 0; --r) {
        if (fwrite(bmp->rows[r], sizeof(pixel_t), (size_t)width, f) != (size_t)width) { fclose(f); return 1; }
        if (padding) {
            if (fwrite(padbuf, 1, padding, f) != padding) { fclose(f); return 1; }
        }
    }

    fclose(f);
    return 0;
}

void free_bmp(bmp_t *bmp) {
    if (!bmp) return;
    if (bmp->fileheader) free(bmp->fileheader);
    if (bmp->infoheader) free(bmp->infoheader);
    if (bmp->rows) {
        free(bmp->rows[0]);
        free(bmp->rows);
    }
    free(bmp);
}
