#include "color_strategy.h"

std::vector<Strategy> strategies = {
    Strategy("bw", [](int x) { return (x == 0) ? 0 : 255; } ),
    Strategy("sin", [](int x) { return 255*sin(M_PI/2 * x/255.0); } ),
    Strategy("log", [](int x) { return (!x) ? (0) : (255*log(x) / log(255)); } ),
    Strategy("tanh", [](int x) { return 256 * tanh(x/128); } ) 
};