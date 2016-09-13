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

uint8_t* naive_mandlebrot(long double x_min, long double x_max, long double y_min, long double y_max, int steps) {
    if(x_min >= x_max || y_min >= y_max) {
        return NULL;
    }

    long double y_stepsize = (y_max - y_min) / steps;
    long double x_stepsize = (x_max - x_min) / steps;

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