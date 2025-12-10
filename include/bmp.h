#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h> 

#ifndef BMP_H_
#define BMP_H_

#pragma pack(push, 1)
typedef struct {

    char B;
    char G;
    char R;

} pixel_t;

typedef struct {
    
    char signature[2];
    int file_size;
    int reserved;
    int file_offset_to_pixels;

} bmp_fileheader_t;

typedef struct {

    int header_size;
    int image_width;
    int image_height;
    int smth1;
    int smth2;
    int image_size;

} bmp_v5header_t;

typedef struct {

    bmp_fileheader_t *fileheader;
    bmp_v5header_t *v5header;

    pixel_t **pixel_array;

} bmp_t;
#pragma pack(pop)

int load_bmp(char *filepath, bmp_t *bitmap);
int crop(bmp_t *bmp, int x, int y, int width, int height);
int rotate(bmp_t *bmp);
void save_bmp(char *filepath, bmp_t *bmp);
void free_bmp(bmp_t *bmp);

#endif