#include "bmp.h"

int main(int argc, char **argv) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s IN_FILE OUT_FILE X Y W H\n", argv[0]);
        return 1;
    }

    const char *in_filepath = argv[1];
    const char *out_filepath = argv[2];

    int X = atoi(argv[3]);
    int Y = atoi(argv[4]);
    int W = atoi(argv[5]);
    int H = atoi(argv[6]);

    if (W <= 0 || H <= 0) {
        fprintf(stderr, "W and H must be positive\n");
        return 1;
    }

    bmp_t *bmp = calloc(1, sizeof(bmp_t));
    if (!bmp) return 1;

    if (load_bmp(in_filepath, bmp)) { fprintf(stderr, "Failed to load BMP\n"); free_bmp(bmp); return 1; }

    if (save_bmp(out_filepath, bmp)) { fprintf(stderr, "Save failed\n"); free_bmp(bmp); return 1; }

    free_bmp(bmp);
    return 0;
}
