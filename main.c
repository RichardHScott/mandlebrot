#include "bitmap.h"
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/* Refactor this into it's own file */

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

        if(complex_magnitude(x,y) > 4) {
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

struct Input_Params {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    int steps;
};

void set_default_input_params(struct Input_Params* params) {
    params->x_min = -0.5;
    params->x_max = 0.3;
    params->y_min = 0.5;
    params->y_max = 1.0;
    params->steps = 1000;

    return;
}

struct Input_Params* parse_args(int argc, char** argv) {
    int i = 0;

    const char x_min_str[] = "xmin=";
    const char x_max_str[] = "xmax=";
    const char y_min_str[] = "ymin=";
    const char y_max_str[] = "ymax=";
    const char steps_str[] = "steps=";

    struct Input_Params* params = (struct Input_Params*) malloc(sizeof(struct Input_Params));

    if(params == NULL) {
        return NULL;
    }

    set_default_input_params(params);

    for(int i = 0; i < argc; ++i) {
        if(argv[i][0] != '-') {
            continue;
        }

        char* argument  = argv[i] + sizeof(char);

        if(0 == strncmp(argument, x_min_str, strlen(x_min_str))) {
            double val = strtod(argument + strlen(x_min_str), NULL);
            if(val != 0) {
                params->x_min = val;
            }
            continue;
        }

        if(0 == strncmp(argument, y_min_str, strlen(y_min_str))) {
            double val = strtod(argument + strlen(y_min_str), NULL);
            if(val != 0) {
                params->y_min = val;
            }
            continue;
        }

        if(0 == strncmp(argument, x_max_str, strlen(x_max_str))) {
            double val = strtod(argument + strlen(x_max_str), NULL);
            if(val != 0) {
                params->x_max = val;
            }
            continue;
        }

        if(0 == strncmp(argument, y_max_str, strlen(y_max_str))) {
            double val = strtod(argument + strlen(y_max_str), NULL);
            if(val != 0) {
                params->y_max = val;
            }
            continue;
        }

        if(0 == strncmp(argument, steps_str, strlen(steps_str))) {
            int val = atoi(argument + strlen(steps_str));
            if(val != 0) {
                params->steps = val;
            }
            continue;
        }
    }

    return params;
}

int main(int argc, char** argv) {
    //Following is the default set for generating the mandlebrot image.
    //It is bounded by the circle |z| = 2, Heine-Borel thus says that it
    //is compact!
    // double x_min = -2;
    // double x_max = 1;
    // double y_min = -1.5;
    // double y_max = 1.5;

    //Below give a nice looking picture
    double x_min = -0.5;
    double x_max = 0.3;
    double y_min = 0.5;
    double y_max = 1.0;


    struct Input_Params* params = parse_args(argc, argv);
    uint8_t* array = naive_mandlebrot(params->x_min, params->x_max, params->y_min, params->y_max, params->steps);

    if(array == NULL) {
        return -1;
    }

    struct Bitmap_Data data;
    uint8_t* pixel_data = (uint8_t*) malloc(3* params->steps* params->steps); 

    data.horizontal_pixels =  params->steps;
    data.vertical_pixels =  params->steps;
    data.bitmapArraySize =  params->steps* params->steps;
    data.bitmapArray = pixel_data;

    for(int i = 0; i < data.bitmapArraySize; ++i) {
        //data.bitmapArray[3*i] = (255*sin(M_PI/2 * array[i]/255.0));
        data.bitmapArray[3*i] = 255*log(array[i]) / log(255);

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