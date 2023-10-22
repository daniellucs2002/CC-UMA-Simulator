#pragma once
#include "State.hpp"

class InvalidState : public State {
public:
    static InvalidState* getInstance();

    std::string getStateName() const override { return "Invalid"; }

private:
    InvalidState() {}
    static InvalidState* instance;
};

class ModifiedState : public State {
public:
    static ModifiedState* getInstance();

    std::string getStateName() const override { return "Modified"; }

private:
    ModifiedState() {}
    static ModifiedState* instance;
};

class ExclusiveState : public State {
public:
    static ExclusiveState* getInstance();

    std::string getStateName() const override { return "Exclusive"; }

private:
    ExclusiveState() {}
    static ExclusiveState* instance;
};

class SharedState : public State {
public:
    static SharedState* getInstance();

    std::string getStateName() const override { return "Shared"; }

private:
    SharedState() {}
    static SharedState* instance;
};
