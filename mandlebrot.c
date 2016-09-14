#include "mandlebrot.h"
#include <complex.h>

static inline long double complex_magnitude(long double real, long double img) {
    return real*real + img*img;
}

uint8_t compute_convergence(long double complex c) {
    long double complex z = 0 + 0*I;
    
    for(int i = 0; i < UINT8_MAX; ++i) {
        if(cabsl(z) > 4) {    
            return i;
        }

        long double complex tmp_z = z;
        z = tmp_z*tmp_z + c;
    }

    return 0;
}

uint8_t compute_convergence_julia(long double complex c) {
    long double complex z = c;

    for(int i = 0; i < UINT8_MAX; ++i) {
        if(cabsl(z) > 9) {    
            return i;
        }

        long double complex tmp_z = z;
        z = tmp_z*tmp_z - 0.8*I;
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
            double long complex z_0 = (x_min + x * x_stepsize) + (y_min + y * y_stepsize) * I;
            array[x * y_divisions + y] = compute_convergence_julia(z_0);
        }
    }

    return array;
}