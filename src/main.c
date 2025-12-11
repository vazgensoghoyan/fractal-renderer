#include "bmp_io.h"
#include "bmp.h"

#include "stdio.h"

void draw_line_dda(bmp_t *bmp, int x0, int y0, int x1, int y1, pixel_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    if (steps == 0) {
        set_pixel(bmp, y0, x0, color);
        return;
    }

    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++) {
        set_pixel(bmp, (int)(y + 0.5f), (int)(x + 0.5f), color);
        x += x_inc;
        y += y_inc;
    }
}

void draw_line_bresenham(bmp_t *bmp, int x0, int y0, int x1, int y1, pixel_t color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (1) {
        set_pixel(bmp, y0, x0, color);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_image(bmp_t *bmp, int width, int height) {
    pixel_t red   = { .R = 255, .G = 0,   .B = 0 };
    pixel_t green = { .R = 0,   .G = 255, .B = 0 };
    pixel_t blue  = { .R = 0,   .G = 0,   .B = 255 };
    pixel_t white = { .R = 255, .G = 255, .B = 255 };

    // 1. заливка фона белым
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            set_pixel(bmp, x, y, white);
        }
    }

    // 2. диагонали через центр (DDA)
    draw_line_dda(bmp, 0, 0, width - 1, height - 1, red);
    draw_line_dda(bmp, 0, height - 1, width - 1, 0, red);

    // 3. горизонтальная и вертикальная линии через центр (Bresenham)
    int cx = width / 2;
    int cy = height / 2;
    draw_line_bresenham(bmp, 0, cy, width - 1, cy, green);  // горизонталь
    draw_line_bresenham(bmp, cx, 0, cx, height - 1, green); // вертикаль

    // 4. сетка каждые 50 пикселей (DDA)
    for (int i = 50; i < width; i += 50) {
        draw_line_dda(bmp, i, 0, i, height - 1, blue); // вертикали
    }
    for (int j = 50; j < height; j += 50) {
        draw_line_dda(bmp, 0, j, width - 1, j, blue); // горизонты
    }
}


void render(const char* output_file) {
    int width = 500;
    int height = 500;

    bmp_t bmp;
    init_empty_bmp(&bmp, width, height);
    
    draw_image(&bmp, width, height);

    bmp_save(&bmp, output_file);

    free_bmp(&bmp);
}

int main(int argc, char** argv) {
    if (argc != 2) { fprintf(stderr, "What are you doing man\n"); return 1; }

    char* output_file = argv[1];

    render(output_file);

    return 0;
}
