#ifndef MANDLEBROT_H
#define MANDLEBROT_H

#include <memory>
#include <cstdint>
#include <complex>

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

        Mandlebrot(Mandlebrot_Parameters const & params);
        ~Mandlebrot();
        Mandlebrot(Mandlebrot const &) = delete;
        Mandlebrot(Mandlebrot&&) = delete;

        uint8_t* naive_mandlebrot();
};

#endif /* MANDLEBROT_H */