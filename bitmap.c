#include "bitmap.h"


static inline uint8_t average(uint8_t* data, int width, int top, int left) {
    uint16_t total = 0;

    total = data[3*width*top + left] + data[3*width*top + left + 3] + data[3*width*(top+1) + left] + data[3*width*(top+1) + left + 3];
    
    return total / 4;
}

uint8_t* one_pixel_blur(uint8_t* data, int width, int height) {
    uint8_t* new_data = (uint8_t*) malloc(width * height * 3);

    if(new_data == NULL) {
        return NULL;
    }

    for(int i = 1; i < height-1; ++i) {
        for(int j = 3; j < 3*width-6; j+=3) {
            new_data[(i-1)*3*width + (j-3)] = average(data, width, i-1, j-3);
            new_data[(i-1)*3*width + (j-3) + 1] = average(data, width, i-1, j-3+1);
            new_data[(i-1)*3*width + (j-3) + 2] = average(data, width, i-1, j-3+2);
        }
    }

    return new_data;
}

Bitmap_Data* four_times_AA(uint8_t* data, int width, int height) {
    int new_width = width / 2;
    int new_height = height / 2;
    uint8_t* new_data = (uint8_t*) malloc(new_width * new_height * 3);

    for(int i = 0; i < new_height-1; ++i) {
        for(int j = 0; j < new_width; ++j) {
            new_data[3*i*new_width + 3*j] = (data[6*i*width + 6*j] + data[6*i*width + 6*(j+1)] + data[6*(i+1)*width + 6*j] + data[6*(i+1)*width + 6*(j+1)])/4;
            new_data[3*i*new_width + 3*j + 1] = (data[6*i*width + 6*j + 1] + data[6*i*width + 6*(j+1) + 1] + data[6*(i+1)*width + 6*j + 1] + data[6*(i+1)*width + 6*(j+1) + 1])/4;
            new_data[3*i*new_width + 3*j + 2] = (data[6*i*width + 6*j + 2] + data[6*i*width + 6*(j+1) + 2] + data[6*(i+1)*width + 6*j + 2] + data[6*(i+1)*width + 6*(j+1) + 2])/4;
        }
    }

    Bitmap_Data* bitmap_data = (Bitmap_Data*) malloc(sizeof(Bitmap_Data));
    bitmap_data->horizontal_pixels = new_width;
    bitmap_data->vertical_pixels =  new_height;
    bitmap_data->bitmapArraySize = new_width*new_height;
    bitmap_data->bitmapArray = new_data;

    return bitmap_data;
}

int save_bitmap_to_disk(Bitmap_Data* data, FILE* file) {
    uint32_t line_padding = (4 - (3 * data->horizontal_pixels) % 4) % 4;

    Bitmap_File_Header file_header;
    DIB_Header dib_header;

    file_header.header = 0x4D42;
    file_header.file_size = sizeof(Bitmap_File_Header) + sizeof(DIB_Header) + 3*(data->bitmapArraySize) + data->vertical_pixels*line_padding;
    file_header.reserved1 = 0;
    file_header.reserved2 = 0;
    file_header.pixel_data_offset = sizeof(Bitmap_File_Header) + sizeof(DIB_Header);

    fwrite(&file_header, sizeof(Bitmap_File_Header), 1, file);
    
    dib_header.header_size = 40;
    dib_header.width = data->horizontal_pixels;
    dib_header.height = data->vertical_pixels;
    dib_header.number_of_color_planes = 1;
    dib_header.bits_per_pixel = 24;
    dib_header.compression_method = 0;
    dib_header.image_size = 0;
    dib_header.horizontal_resolution = 1024;
    dib_header.vertical_resolution = 1024;
    dib_header.number_of_colors_in_color_palette = 0;
    dib_header.number_of_important_colors_used = 0;

    fwrite(&dib_header, sizeof(DIB_Header), 1, file);
    
    int i = 0;
    uint32_t zero = 0;
    for(i = 0; i < data->vertical_pixels; ++i) {
        fwrite(3*i*data->horizontal_pixels + data->bitmapArray, sizeof(uint8_t), 3*data->horizontal_pixels, file);
        fwrite(&zero, sizeof(uint8_t), line_padding, file);
    }

    return 0;
}

int save_bitmap(Bitmap *bitmap, char* filename) {
    FILE* file = fopen(filename, "wb");

    if(file != NULL) {
        save_bitmap_to_disk(bitmap->data, file);
        fclose(file);
    } else {
        return -1;
    }

    return 0;
}

Bitmap* create_bitmap(uint32_t horizontal_dim, uint32_t vertical_dim, uint8_t* data) {
    Bitmap* bitmap = (Bitmap*) malloc(sizeof(Bitmap));

    if(bitmap == NULL) {
        return NULL;
    }

    Bitmap_Data *bitmap_data = (Bitmap_Data*) malloc(sizeof(Bitmap_Data));

    if (bitmap_data != NULL) {
        bitmap_data->horizontal_pixels = horizontal_dim;
        bitmap_data->vertical_pixels = vertical_dim;
        bitmap_data->bitmapArray = data;
        bitmap_data->bitmapArraySize = horizontal_dim * vertical_dim;

        bitmap->data = bitmap_data;
    } else {
        free(bitmap);
    }

    return bitmap;
}