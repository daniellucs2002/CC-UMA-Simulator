#include "MoesiState.hpp"

OwnedState* OwnedState::instance = nullptr;

OwnedState* OwnedState::getInstance() {
    if (instance == nullptr) {
        instance = new OwnedState();
    }
    return instance;
}
