#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef struct {
    uint32_t horizontal_pixels;
    uint32_t vertical_pixels;
    uint8_t* bitmapArray;
    uint32_t bitmapArraySize;
} Bitmap_Data;

typedef struct {
    uint16_t header;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixel_data_offset;
} __attribute__((packed)) Bitmap_File_Header;

typedef struct {
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t number_of_color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t image_size;
    int32_t horizontal_resolution;
    int32_t vertical_resolution;
    uint32_t number_of_colors_in_color_palette;
    uint32_t number_of_important_colors_used;
}  __attribute__((packed)) DIB_Header;

typedef struct {
    Bitmap_File_Header *file_header;
    DIB_Header *dib_header;
    Bitmap_Data *data;
} Bitmap;

Bitmap* create_bitmap(uint32_t horitzontal_dim, uint32_t vertical_dim, uint8_t* data);
int save_bitmap(Bitmap *bitmap, char* filename);

#endif /* BITMAP_H */