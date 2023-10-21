#pragma once
#include <string>
#include "cache/CacheLine.hpp"

class State {
public:
    virtual std::string getStateName() const = 0;
};
