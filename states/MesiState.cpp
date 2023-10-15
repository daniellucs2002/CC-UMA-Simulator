#include "MesiState.hpp"
#include <cassert>

InvalidState* InvalidState::instance = nullptr;

InvalidState* InvalidState::getInstance() {
    if (instance == nullptr) {
        instance = new InvalidState();
    }
    return instance;
}

ModifiedState* ModifiedState::instance = nullptr;

ModifiedState* ModifiedState::getInstance() {
    if (instance == nullptr) {
        instance = new ModifiedState();
    }
    return instance;
}

ExclusiveState* ExclusiveState::instance = nullptr;

ExclusiveState* ExclusiveState::getInstance() {
    if (instance == nullptr) {
        instance = new ExclusiveState();
    }
    return instance;
}

SharedState* SharedState::instance = nullptr;

SharedState* SharedState::getInstance() {
    if (instance == nullptr) {
        instance = new SharedState();
    }
    return instance;
}

void InvalidState::transitionTo(const std::string& newState, CacheLine& cacheline) {
    if (newState == "Modified")
        cacheline.setState(ModifiedState::getInstance());
    else if (newState == "Exclusive")
        cacheline.setState(ExclusiveState::getInstance());
    else if (newState == "Shared")
        cacheline.setState(SharedState::getInstance());
    else
        assert(false);
}

void ModifiedState::transitionTo(const std::string& newState, CacheLine& cacheline) {
    if (newState == "Invalid")
        cacheline.setState(InvalidState::getInstance());
    else if (newState == "Exclusive")
        cacheline.setState(ExclusiveState::getInstance());
    else if (newState == "Shared")
        cacheline.setState(SharedState::getInstance());
    else
        assert(false);
}

void ExclusiveState::transitionTo(const std::string& newState, CacheLine& cacheline) {
    if (newState == "Invalid")
        cacheline.setState(InvalidState::getInstance());
    else if (newState == "Shared")
        cacheline.setState(SharedState::getInstance());
    else if (newState == "Modified")
        cacheline.setState(ModifiedState::getInstance());
    else
        assert(false);
}

void SharedState::transitionTo(const std::string& newState, CacheLine& cacheline) {
    if (newState == "Invalid")
        cacheline.setState(InvalidState::getInstance());
    else if (newState == "Modified")
        cacheline.setState(ModifiedState::getInstance());
    else if (newState == "Exclusive")
        cacheline.setState(ExclusiveState::getInstance());
    else
        assert(false);
}
