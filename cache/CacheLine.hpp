#pragma once
#include "states/State.hpp"
#include "states/MesiState.hpp"

class State;
class CacheSet;

class CacheLine {
    friend class CacheSet;
private:
    bool is_valid;
    unsigned int tag;
    bool is_dirty;

    // cache line state that is pointed to
    State* state;

public:
    CacheLine() : is_valid(false), tag(0), is_dirty(false) { }

    State* getState() const { return state; }
    void setState(State* newState) { state = newState; }

    void setValid(bool newBit) { is_valid = newBit; }
};
