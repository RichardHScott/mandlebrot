#include "mandlebrot.h"

static inline long double complex_magnitude(long double real, long double img) {
    return real*real + img*img;
}

uint8_t compute_convergence(long double c_real, long double c_img) {
    long double x = 0;
    long double y = 0;

    for(int i = 0; i < UINT8_MAX; ++i) {
        if(complex_magnitude(x,y) > 4) {    
            return i;
        }

        long double tmp_x = x;
        x = tmp_x*tmp_x - y*y + c_real;
        y = 2*tmp_x*y + c_img;
    }

    return 0;
}

uint8_t compute_convergence_julia(long double c_real, long double c_img) {
    long double x = c_real;
    long double y = c_img;

    for(int i = 0; i < UINT8_MAX; ++i) {
        if(complex_magnitude(x,y) > 4) {    
            return i;
        }

        long double tmp_x = x;
        x = tmp_x*tmp_x - y*y;
        y = 2*tmp_x*y + 0.8;
    }

    return 0;
}

uint8_t* naive_mandlebrot(long double x_min, long double x_max, long double y_min, long double y_max, int x_divisions, int y_divisions) {
    if(x_min >= x_max || y_min >= y_max) {
        return NULL;
    }

    long double y_stepsize = (y_max - y_min) / y_divisions;
    long double x_stepsize = (x_max - x_min) / x_divisions;

    size_t array_size = y_divisions*x_divisions;
    uint8_t* array = (uint8_t*) malloc(array_size);

    if(array == NULL) {
        return NULL;
    }

    for(int x = 0; x < x_divisions; ++x) {
        for(int y = 0; y < y_divisions; ++y) {
            array[x * y_divisions + y] = compute_convergence_julia(x_min + x * x_stepsize, y_min + y * y_stepsize);
        }
    }

    return array;
}