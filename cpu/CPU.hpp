#include "timer/Timer.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

class CPU : public Observer {
private:
    int cpu_id;
    string filename;
    std::ifstream inFile;  // trace file for the current cpu core

public:
    CPU(int id, string filename);
    bool update(unsigned long now) override;
    ~CPU();
};
