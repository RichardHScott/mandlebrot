#ifndef MANDLEBROT_H
#define MANDLEBROT_H

#include <malloc.h>
#include <stdint.h>

uint8_t* naive_mandlebrot(long double x_min, long double x_max, long double y_min, long double y_max, int x_divisions, int y_divisions);

#endif /* MANDLEBROT_H */