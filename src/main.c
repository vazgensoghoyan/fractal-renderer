#include "bmp.h"

int main(int argc, char **argv) {
    char *target = argv[1];

    char *IN_FILEPATH = argv[2];
    char *OUT_FILEPATH = argv[3];

    int X = atoi(argv[4]);
    int Y = atoi(argv[5]);
    int W = atoi(argv[6]);
    int H = atoi(argv[7]);

    bmp_t *bitmap = malloc(sizeof(bmp_t));

    if (load_bmp(IN_FILEPATH, bitmap))
        return 1;

    if (crop(bitmap, X, Y, W, H))
        return 1;

    if (rotate(bitmap))
        return 1;

    save_bmp(OUT_FILEPATH, bitmap);

    free_bmp(bitmap);

    return 0;
}