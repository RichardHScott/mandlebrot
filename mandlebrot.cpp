#include "mandlebrot.h"

#include <chrono>
#include <iostream>

using namespace std;


// requires 
// width mod 4 = 0
void vectorized(std::unique_ptr<uint8_t[]> const & array, 
    int y_steps, double start_y, double end_y, 
    int x_steps, double d_start_x, double d_end_x ) {

    // int y_steps = 4000;
    // double start_y = -2.0;
    // double end_y = 2.0;
    double d_dy = (end_y - start_y) / (double)y_steps; 

    __m256d dy = _mm256_broadcast_sd(&d_dy);
    __m256d curr_y = _mm256_broadcast_sd(&start_y);

    // double d_start_x = -1.0;
    // double d_end_x = 1.0;
    // int x_steps = 4000;
    double d_dx = (d_end_x - d_start_x) / (double)x_steps;

    __m256d start_x = _mm256_broadcast_sd(&d_start_x);
    __m256d start_d_x = _mm256_set_pd(0, d_dx, 2*d_dx, 3*d_dx);
    start_x = _mm256_add_pd(start_x, start_d_x);

    double const d_dx_4 = 4*d_dx;
    __m256d four_d_dx = _mm256_broadcast_sd(&d_dx_4);

    double const four = 4;
    __m256d fours = _mm256_broadcast_sd(&four);

    double const one = 1.0;
    __m256d ones = _mm256_broadcast_sd(&one);

    int max_iters = 256;

    for(int j = 0; j < y_steps; ++j) {
        __m256d curr_x = start_x;

        for(int i = 0; i < x_steps; i+=4) {
            //reset y value
            __m256d y = curr_y;
            __m256d x = curr_x;

            int iter = 0;
            int norm_greater_than_four = 0;

            __m256d individual_iter = _mm256_xor_pd(x,x);
            //loop through iters
            do {
                __m256d x_squared = _mm256_mul_pd(x, x);
                __m256d y_squared = _mm256_mul_pd(y, y);

                //compare and add iter
                __m256d norm = _mm256_add_pd(x_squared, y_squared);
                norm = _mm256_cmp_pd(norm, fours, _CMP_LT_OQ);
                norm_greater_than_four = _mm256_movemask_pd(norm) & 255;
                norm = _mm256_and_pd(norm, ones);
                individual_iter = _mm256_add_pd(individual_iter, norm);

                //y_new = 2*x*y + c_y
                y = _mm256_mul_pd(x, y);
                y = _mm256_add_pd(y, y);
                y = _mm256_add_pd(y, curr_y);

                //x_old = x^2 + y^2 + old c_x
                x = _mm256_sub_pd(x_squared, y_squared);
                x = _mm256_add_pd(x, curr_x);

                ++iter;
            } while( (norm_greater_than_four != 0) && (iter < max_iters) );

            __m128i iterations = _mm256_cvtpd_epi32(individual_iter);

            array[i + j*x_steps] = ((uint8_t*)&iterations)[12];
            array[1 + i + j*x_steps] = ((uint8_t*)&iterations)[8];
            array[2 + i + j*x_steps] = ((uint8_t*)&iterations)[4];
            array[3 + i + j*x_steps] = ((uint8_t*)&iterations)[0];

            //increase to next set of four
            curr_x = _mm256_add_pd(curr_x, four_d_dx);
        }

        //after loop increase y
        curr_y = _mm256_add_pd(curr_y, dy);
    }
}

static inline uint8_t compute_convergence(complex<double> c) {
    complex<double> z(0, 0);
    
    for(int i = 0; i < UINT8_MAX; ++i) {
        if(abs(z) > 4) {    
            return i;
        }

        z = (z*z) + c;
    }

    return 0;
}

static inline uint8_t compute_convergence_julia(complex<double> c) {
    complex<double> z = c;

    for(int i = 0; i < UINT8_MAX; ++i) {
        if(abs(z) > 9) {    
            return i;
        }

        z = (z*z) - complex<double>(0, 0.8);
    }

    return 0;
}

Mandlebrot::Mandlebrot(Mandlebrot_Parameters const & parameters) {
    params = make_unique<Mandlebrot_Parameters>();

    params->x_max = parameters.x_max;
    params->x_min = parameters.x_min;
    params->y_min = parameters.y_min;
    params->y_max = parameters.y_max;
    params->x_divisions = parameters.x_divisions;
    params->y_divisions = parameters.y_divisions;
}

unique_ptr<uint8_t[]> Mandlebrot::naive_mandlebrot() {
    double x_max = params->x_max;
    double x_min = params->x_min;
    double y_min = params->y_min;
    double y_max = params->y_max;
    int x_divisions = params->x_divisions;
    int y_divisions = params->y_divisions;

    if(x_min >= x_max || y_min >= y_max) {
        return NULL;
    }

    double y_stepsize = (y_max - y_min) / y_divisions;
    double x_stepsize = (x_max - x_min) / x_divisions;

    size_t array_size = y_divisions*x_divisions;
    auto array = make_unique<uint8_t[]>(array_size);

    if(array == NULL) {
        return NULL;
    }
    
   using milli = std::chrono::milliseconds;

    auto start_vectorized = std::chrono::high_resolution_clock::now();
    vectorized(array,
    y_divisions, y_min, y_max,
    x_divisions, x_min, x_max);
    auto end_vectorized = std::chrono::high_resolution_clock::now();
    std::cout << "Vector time: " << (std::chrono::duration<double>(end_vectorized - start_vectorized)).count() << "\r\n";

    return array;
}
