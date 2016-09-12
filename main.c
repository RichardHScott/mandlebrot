#include "bitmap.h"
#include <malloc.h>

double complex_magnitude(double real, double img) {
    return real*real + img*img;
}

uint8_t compute_convergence(double c_real, double c_img) {
    double x = 0;
    double y = 0;

    for(int i = 0; i < 255; ++i) {
        double tmp_x = x;
        x = tmp_x*tmp_x - y*y + c_real;
        y = 2*tmp_x*y + c_img;

        if(complex_magnitude(x,y) > 10) {
            return i;
        }
    }

    return 0;
}

uint8_t* naive_mandlebrot(double x_min, double x_max, double y_min, double y_max, int steps) {
    if(x_min >= x_max || y_min >= y_max) {
        return NULL;
    }

    double y_stepsize = (y_max - y_min) / steps;
    double x_stepsize = (x_max - x_min) / steps;

    size_t array_size = steps*steps;
    uint8_t* array;
    array = (uint8_t*) malloc(array_size);

    if(array == NULL) {
        return NULL;
    }

    int x, y;
    for(x = 0; x < steps; ++x) {
        for(y = 0; y < steps; ++y) {
            array[x*steps + y] = compute_convergence(x_min + x * x_stepsize, y_min + y * y_stepsize);
        }
    }

    return array;
}

int main(int argc, char** argv) {
    double x_min = -2;
    double x_max = 1;
    double y_min = -1.5;
    double y_max = 1.5;
    int steps = 2000;
    uint8_t* array = naive_mandlebrot(x_min, x_max, y_min, y_max, steps);

    if(array == NULL) {
        return -1;
    }

    struct Bitmap_Data data;
    uint8_t* pixel_data = (uint8_t*) malloc(3*steps*steps); 

    data.horizontal_pixels = steps;
    data.vertical_pixels = steps;
    data.bitmapArraySize = steps*steps;
    data.bitmapArray = pixel_data;

    for(int i = 0; i < data.bitmapArraySize; ++i) {
        data.bitmapArray[3*i] = array[i];
        data.bitmapArray[3*i+1] = 0;
        data.bitmapArray[3*i+2] = 0;
    }

    FILE* file = fopen("temp.bmp", "wb");
    save_bitmap_to_disk(&data, file);
    fclose(file);

    free(array);
    free(pixel_data);
    return 0;
}