#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t horizontal_pixels;
    uint32_t vertical_pixels;
    uint8_t* bitmapArray;
    uint32_t bitmapArraySize;
} Bitmap_Data;

int save_bitmap_to_disk(Bitmap_Data *data_ptr, FILE* file);

#endif /* BITMAP_H */