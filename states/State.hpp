#pragma once
#include <string>

class State {
public:
    virtual std::string getStateName() const = 0;
};
