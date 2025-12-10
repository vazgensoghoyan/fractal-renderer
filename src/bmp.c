#include "bmp.h"

/* --- utilities --- */

static size_t row_byte_width(uint16_t bits_per_pixel, int32_t width) {
    return (((size_t)bits_per_pixel * (size_t)width + 31u) / 32u) * 4u;
}

static int safe_fread(void *ptr, size_t size, size_t nmemb, FILE *f) {
    return fread(ptr, size, nmemb, f) == nmemb ? 0 : 1;
}

/* --- core functions --- */

int init_empty_bmp(bmp_t *bmp, int width, int height) {
    if (!bmp || width <= 0 || height <= 0)
        return 1;

    bmp->fileheader = malloc(sizeof(bmp_fileheader_t));
    bmp->infoheader = malloc(sizeof(bmp_infoheader_t));
    bmp->rows = NULL;

    if (!bmp->fileheader || !bmp->infoheader)
        return 1;

    // Prepare headers
    bmp_fileheader_t *fh = bmp->fileheader;
    bmp_infoheader_t *ih = bmp->infoheader;

    const int bpp = 24;
    size_t row_bytes = ((width * 3 + 3) / 4) * 4;
    size_t image_size = row_bytes * (size_t)height;

    fh->signature = 0x4D42; // 'BM'
    fh->reserved1 = 0;
    fh->reserved2 = 0;
    fh->file_offset_to_pixels = sizeof(bmp_fileheader_t) + sizeof(bmp_infoheader_t);
    fh->file_size = fh->file_offset_to_pixels + image_size;

    ih->header_size = 40;
    ih->image_width = width;
    ih->image_height = height;        // bottom-up (positive)
    ih->planes = 1;
    ih->bits_per_pixel = bpp;
    ih->compression = 0;
    ih->image_size = image_size;
    ih->x_pixels_per_meter = 2835;    // ~72 DPI
    ih->y_pixels_per_meter = 2835;
    ih->colors_used = 0;
    ih->important_colors = 0;

    // Allocate pixel buffer
    pixel_t *pixels = calloc((size_t)width * height, sizeof(pixel_t));
    pixel_t **rows = calloc(height, sizeof(pixel_t *));
    if (!pixels || !rows) {
        free(pixels);
        free(rows);
        return 1;
    }

    for (int i = 0; i < height; i++)
        rows[i] = pixels + i * width;

    bmp->rows = rows;
    return 0;
}

int load_bmp(const char *filepath, bmp_t *bmp) {
    if (!filepath || !bmp) return 1;
    FILE *f = fopen(filepath, "rb");
    if (!f) return 1;

    bmp->fileheader = malloc(sizeof(bmp_fileheader_t));
    bmp->infoheader = malloc(sizeof(bmp_infoheader_t));
    bmp->rows = NULL;
    if (!bmp->fileheader || !bmp->infoheader) { fclose(f); free(bmp->fileheader); free(bmp->infoheader); return 1; }

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
    if (row_bytes < pixel_row_bytes) goto fail; // defensive
    size_t padding = row_bytes - pixel_row_bytes;

    /* allocate contiguous pixel buffer and row pointers */
    pixel_t *pixels = calloc(abs_width * abs_height, sizeof(pixel_t));
    pixel_t **rows = calloc(abs_height, sizeof(pixel_t *));
    if (!pixels || !rows) { free(pixels); free(rows); goto fail; }

    for (size_t r = 0; r < abs_height; ++r) {
        rows[r] = pixels + r * abs_width;
    }

    if (fseek(f, bmp->fileheader->file_offset_to_pixels, SEEK_SET) != 0) { free(pixels); free(rows); goto fail; }

    for (size_t i = 0; i < abs_height; ++i) {
        size_t target_row = top_down ? i : (abs_height - 1 - i);
        if (fread(rows[target_row], sizeof(pixel_t), abs_width, f) != abs_width) { free(pixels); free(rows); goto fail; }
        if (padding) {
            if (fseek(f, (long)padding, SEEK_CUR) != 0) { free(pixels); free(rows); goto fail; }
        }
    }

    /* Normalize: we store rows[0] == top row, so make infoheader->image_height positive */
    bmp->infoheader->image_height = (int32_t)abs_height;

    bmp->rows = rows;
    fclose(f);
    return 0;

fail:
    fclose(f);
    free(bmp->fileheader);
    free(bmp->infoheader);
    bmp->fileheader = NULL;
    bmp->infoheader = NULL;
    return 1;
}

int set_pixel(bmp_t *bmp, int i, int j, pixel_t color) {
    if (!bmp || !bmp->infoheader || !bmp->rows) return 1;

    int w = bmp->infoheader->image_width;
    int h = bmp->infoheader->image_height;

    if (h < 0) h = -h;

    if (i < 0 || j < 0 || i >= h || j >= w) return 1;

    bmp->rows[i][j] = color;
    return 0;
}

int put_rgb(bmp_t *bmp, int i, int j, uint8_t r, uint8_t g, uint8_t b) {
    pixel_t p = { .B = b, .G = g, .R = r };
    return set_pixel(bmp, i, j, p);
}

int get_pixel(bmp_t *bmp, int i, int j, pixel_t *out_color) {
    if (!bmp || !bmp->infoheader || !bmp->rows || !out_color)
        return 1;

    int w = bmp->infoheader->image_width;
    int h = bmp->infoheader->image_height;
    if (h < 0) h = -h;

    if (i < 0 || j < 0 || i >= h || j >= w)
        return 1;

    *out_color = bmp->rows[i][j];
    return 0;
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
    if (row_bytes < pixel_row_bytes) { fclose(f); return 1; }
    size_t padding = row_bytes - pixel_row_bytes;
    size_t image_size = row_bytes * (size_t)height;

    /* prepare headers to write; ensure signature is 'BM' */
    bmp_fileheader_t fh = *bmp->fileheader;
    bmp_infoheader_t ih = *bmp->infoheader;

    ih.image_height = height; /* positive => bottom-up */
    ih.image_size = (uint32_t)image_size;

    fh.signature = 0x4D42; /* 'BM' */
    fh.file_offset_to_pixels = (uint32_t)(sizeof(bmp_fileheader_t) + ih.header_size);
    fh.file_size = fh.file_offset_to_pixels + (uint32_t)image_size;

    /* write headers */
    if (fwrite(&fh, sizeof(bmp_fileheader_t), 1, f) != 1) { fclose(f); return 1; }
    if (fwrite(&ih, sizeof(bmp_infoheader_t), 1, f) != 1) { fclose(f); return 1; }

    uint8_t padbuf[4] = {0,0,0,0};

    /* write rows bottom-up: last (bottom) -> first (top) */
    for (int r = height - 1; r >= 0; --r) {
        if (fwrite(bmp->rows[r], sizeof(pixel_t), (size_t)width, f) != (size_t)width) { fclose(f); return 1; }
        if (padding) {
            if (fwrite(padbuf, 1, padding, f) != padding) { fclose(f); return 1; }
        }
    }

    fclose(f);
    return 0;
}

int save_pixels_to_bmp(const char *filepath, pixel_t **rows, int width, int height) {
    if (!filepath || !rows || width <= 0 || height <= 0) return 1;

    FILE *f = fopen(filepath, "wb");
    if (!f) return 1;

    // compute row padding
    int row_bytes = width * 3;
    int padding = (4 - (row_bytes % 4)) % 4;
    int image_size = (row_bytes + padding) * height;

    // prepare headers
    bmp_fileheader_t fh = {
        .signature = 0x4D42, // 'BM'
        .file_size = sizeof(bmp_fileheader_t) + sizeof(bmp_infoheader_t) + image_size,
        .reserved1 = 0,
        .reserved2 = 0,
        .file_offset_to_pixels = sizeof(bmp_fileheader_t) + sizeof(bmp_infoheader_t)
    };

    bmp_infoheader_t ih = {
        .header_size = 40,
        .image_width = width,
        .image_height = height,       // BMP requires bottom-up
        .planes = 1,
        .bits_per_pixel = 24,
        .compression = 0,
        .image_size = image_size,
        .x_pixels_per_meter = 2835,           // 72 DPI
        .y_pixels_per_meter = 2835,
        .colors_used = 0,
        .important_colors = 0
    };

    // write headers
    fwrite(&fh, sizeof(fh), 1, f);
    fwrite(&ih, sizeof(ih), 1, f);

    uint8_t zero[3] = {0, 0, 0};

    // write pixel rows bottom-up
    for (int r = height - 1; r >= 0; r--) {
        fwrite(rows[r], sizeof(pixel_t), width, f);
        fwrite(zero, 1, padding, f);
    }

    fclose(f);
    return 0;
}

void free_bmp(bmp_t *bmp) {
    if (!bmp) return;
    if (bmp->fileheader) { free(bmp->fileheader); bmp->fileheader = NULL; }
    if (bmp->infoheader) { free(bmp->infoheader); bmp->infoheader = NULL; }
    if (bmp->rows) {
        if (bmp->rows[0]) free(bmp->rows[0]);
        free(bmp->rows);
        bmp->rows = NULL;
    }
    free(bmp);
}
