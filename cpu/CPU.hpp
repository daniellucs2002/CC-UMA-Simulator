#pragma once

#include "timer/Timer.hpp"
#include "cache/Cache.hpp"
#include "traces/Trace.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>

using namespace std;

class Observer {
public:
    virtual bool update(unsigned long now) = 0;
    virtual ~Observer() = default;
};

template <typename T>
class EnableShared : public std::enable_shared_from_this<T> {

};

class Trace;

class CPU : public Observer, public EnableShared<CPU> {
private:
    int cpu_id;
    int halt;  // number of cycles, for a blocking cache
    string filename;
    std::ifstream inFile;  // trace file for the current cpu core

    std::shared_ptr<Cache> cache;  // L1 data cache

    std::shared_ptr<Trace> inst;

public:
    CPU(int id, string filename);
    bool update(unsigned long now) override;
    ~CPU();
    std::shared_ptr<Cache> read_cache() {return this->cache;}

    void setHalt(int halt) {this->halt = halt;}

    std::shared_ptr<Trace> getInst() {return this->inst;}

};
