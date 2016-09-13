#include "bitmap.h"

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