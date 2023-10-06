#include "cpu/CPU.hpp"
#include "config.hpp"

extern string root_path;

CPU::CPU(int id, string filename) {
    this->cpu_id = id;
    this->filename = root_path + filename + to_string(id) + ".data";
    // cout << "Constructing CPU " << this->cpu_id << "..." << endl;
    this->inFile.open(this->filename);
    assert(this->inFile.is_open());
}

bool CPU::update(unsigned long now) {
    // read a line from the trace file until it finishes
    string line;
    if (!this->inFile.is_open())
        return false;
    if (getline(inFile, line))
        return true;
    // print out the end of execution time for the current core
    cout << "[CPU " + to_string(this->cpu_id) + "] Finished at cycle " << now << endl;
    inFile.close();
    return false;
}

CPU::~CPU() {
    if (inFile.is_open())
        inFile.close();
}
