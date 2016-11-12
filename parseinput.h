#ifndef _PARSEINPUT_H
#define _PARSEINPUT_H

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "color_strategy.h"

//const std::vector<std::string> Color_Strategies{"bw", "int", "sin", "log", "tanh"};

struct Input_Params {
    double x_min;
    double y_min;
    double x_max;
    double y_max;
    int x_divisions;
    int y_divisions;

    Strategy strategy = strategies[2];

    Input_Params(double xmin = -0.5, double xmax = 0.3, double ymin = 0.5, double ymax = 1.0, int xdivisions = 2000, int ydivisions = 2000)
      : x_min(xmin), y_min(ymin), x_max(xmax), y_max(ymax), x_divisions(xdivisions), y_divisions(ydivisions) { }  
};

class ParseInput {
    private:
    protected:

    public:
        static std::unique_ptr<Input_Params> Parse(int argc, char** argv);
        static std::unique_ptr<Input_Params> parse_args(int argc, char** argv);
};

#endif /* _PARSEINPUT_H */