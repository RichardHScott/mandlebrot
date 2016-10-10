#include "mandlebrot.h"

using namespace std;

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

Mandlebrot::~Mandlebrot() {

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

    for(int x = 0; x < x_divisions; ++x) {
        for(int y = 0; y < y_divisions; ++y) {
            complex<double> z_0(x_min + x * x_stepsize, y_min + y * y_stepsize);
            array[x * y_divisions + y] = compute_convergence(z_0);
        }
    }

    return std::move(array);
}