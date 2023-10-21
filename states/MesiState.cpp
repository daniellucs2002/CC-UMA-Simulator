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
