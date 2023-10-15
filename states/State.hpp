#pragma once
#include <string>
#include "cache/CacheLine.hpp"

class State {
public:
    virtual void transitionTo(const std::string& newState, class CacheLine& cacheline) = 0;
    virtual std::string getStateName() const = 0;
};
