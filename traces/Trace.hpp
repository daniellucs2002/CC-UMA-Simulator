#pragma once
#include <iostream>
#include <memory>
#include <sstream>
#include "cache/Cache.hpp"

class Trace {
public:
    virtual unsigned int detect(std::shared_ptr<Cache> cache) const = 0;
    virtual unsigned int execute(std::shared_ptr<Cache> cache) const = 0;
    virtual int identify() const = 0;
    virtual unsigned int getVal() const = 0;
    virtual ~Trace() {}

    // Factory method to create instruction objects based on input string
    static std::shared_ptr<Trace> createInstruction(const std::string & line);
};

// load instruction
class Instruction0 : public Trace {
private:
    unsigned int address;

public:
    Instruction0(int val) : address(val) { }
    unsigned int execute(std::shared_ptr<Cache> cache) const override;
    unsigned int detect(std::shared_ptr<Cache> cache) const override;
    int identify() const override {return 0;}
    unsigned int getVal() const override {return this->address;}
};

// store instruction
class Instruction1 : public Trace {
private:
    unsigned int address;

public:
    Instruction1(int val) : address(val) { }
    unsigned int execute(std::shared_ptr<Cache> cache) const override;
    unsigned int detect(std::shared_ptr<Cache> cache) const override;
    int identify() const override {return 1;}
    unsigned int getVal() const override {return this->address;}
};

// other instructions
class Instruction2 : public Trace {
private:
    unsigned int time;

public:
    Instruction2(int val) : time(val) { }
    unsigned int execute(std::shared_ptr<Cache> cache) const override;
    unsigned int detect(std::shared_ptr<Cache> cache) const override;
    int identify() const override {return 2;}
    unsigned int getVal() const override {return this->time;}
};
