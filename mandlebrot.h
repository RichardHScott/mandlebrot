#ifndef MANDLEBROT_H
#define MANDLEBROT_H

#include <malloc.h>
#include <stdint.h>

typedef struct {
    long double x_min;
    long double x_max;
    long double y_min;
    long double y_max;
    long double x_divisions;
    long double y_divisions;
} Mandlebrot_Parameters;

uint8_t* naive_mandlebrot(Mandlebrot_Parameters *params);

#endif /* MANDLEBROT_H */