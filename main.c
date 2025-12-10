#include "bmp.h"
#include "stdio.h"

int main(int argc, char** argv) {
    if (argc != 2) { fprintf(stderr, "What are you doing man\n"); return 1; }

    char* output_file = argv[1];

    int width = 640;
    int height = 480;

    pixel_t *pix = calloc(width * height, sizeof(pixel_t));
    pixel_t **rows = calloc(height, sizeof(pixel_t*));

    for (int y = 0; y < height; y++)
        rows[y] = pix + y * width;
    
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            rows[y][x].R = x % 256;
            rows[y][x].G = y % 256;
            rows[y][x].B = (x+y) % 256;
        }
    }

    // сохраняем
    save_pixels_to_bmp(output_file, rows, width, height);

    free(rows[0]);
    free(rows);

    return 0;
}
