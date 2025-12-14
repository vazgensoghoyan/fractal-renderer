#ifndef BMP_IO_H
#define BMP_IO_H

#include "bmp.hpp"

int bmp_load(bmp_t *bmp, const char *filename);
int bmp_save(bmp_t *bmp, const char *filename);

#endif
