#ifndef MANDLEBROT_H
#define MANDLEBROT_H

#include <memory>
#include <cstdint>

#include <chrono>
#include <iostream>

#include <immintrin.h>


class Mandlebrot {
private:
protected:
public:
    typedef struct {
        long double x_min;
        long double x_max;
        long double y_min;
        long double y_max;
        long double x_divisions;
        long double y_divisions;
    } Mandlebrot_Parameters;

    std::unique_ptr<Mandlebrot_Parameters> params;

    Mandlebrot(std::unique_ptr<Mandlebrot_Parameters> parameters) : params(std::move(parameters)) {};

    std::unique_ptr<uint8_t[]> naive_mandlebrot();
};

#endif /* MANDLEBROT_H */