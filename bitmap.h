#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdio.h>

struct Bitmap_Data {
    uint32_t horizontal_pixels;
    uint32_t vertical_pixels;
    uint8_t* bitmapArray;
    uint32_t bitmapArraySize;
};

int save_bitmap_to_disk(struct Bitmap_Data *data_ptr, FILE* file);

#endif