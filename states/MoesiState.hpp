#pragma once
#include "State.hpp"

class OwnedState : public State {
public:
    static OwnedState* getInstance();

    std::string getStateName() const override { return "Owned"; }

private:
    OwnedState() {}
    static OwnedState* instance;
};
