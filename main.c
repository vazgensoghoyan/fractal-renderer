#include "bmp.h"
#include "stdio.h"

void draw_image(bmp_t *bmp, int width, int height) {
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            if (x < 100 && y < 100) continue;

            set_pixel_rgb(bmp, x, y, 255, y % 256, (x + y) % 256);
        }
    }
}

void render(const char* output_file) {
    int width = 640;
    int height = 480;

    bmp_t bmp;
    init_empty_bmp(&bmp, width, height);
    
    draw_image(&bmp, width, height);

    save_bmp(&bmp, output_file);

    free_bmp(&bmp);
}

int main(int argc, char** argv) {
    
    if (argc != 2) { fprintf(stderr, "What are you doing man\n"); return 1; }

    char* output_file = argv[1];

    render(output_file);

    return 0;
}
