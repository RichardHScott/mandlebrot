#ifndef COLOR_STRATEGY_H
#define COLOR_STRATEGY_H

#include <string>
#include <functional>
#include <cmath>
#include <vector>

class Strategy {
    public:
    Strategy(std::string const & name, std::function<int(int)> func) : m_name(name), m_function(func) {}
    std::string m_name;
    std::function<int(int)> m_function;
    
    protected:
    private:
};

extern std::vector<Strategy> strategies;
#endif /* COLOR_STRATEGY_H */