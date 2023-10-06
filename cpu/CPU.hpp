#include "timer/Timer.hpp"
#include "cache/Cache.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

class CPU : public Observer {
private:
    int cpu_id;
    string filename;
    std::ifstream inFile;  // trace file for the current cpu core

    std::shared_ptr<Cache> cache;  // L1 data cache

public:
    CPU(int id, string filename);
    bool update(unsigned long now) override;
    ~CPU();
};
