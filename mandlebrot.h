#ifndef MANDLEBROT_H
#define MANDLEBROT_H

#include <malloc.h>
#include <stdint.h>

uint8_t* naive_mandlebrot(double x_min, double x_max, double y_min, double y_max, int steps);

#endif /* MANDLEBROT_H */