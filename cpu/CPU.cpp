#include "cpu/CPU.hpp"
#include "config.hpp"
#include "traces/Trace.hpp"

extern string root_path;

CPU::CPU(int id, string filename) {
    this->cpu_id = id;
    this->halt = 0;  // default, current instruction lasts for one cycle
    this->filename = root_path + filename + to_string(id) + ".data";
    // cout << "Constructing CPU " << this->cpu_id << "..." << endl;
    this->inFile.open(this->filename);
    assert(this->inFile.is_open());

    // create the unique_ptr to L1 data cache
    this->cache = std::make_shared<Cache>(CacheConfig::cachesize,
        CacheConfig::associativity, CacheConfig::blocksize);
}

bool CPU::update(unsigned long now) {
    // read a line from the trace file until it finishes
    string line;
    if (!this->inFile.is_open())
        return false;
    if (this->halt == 0) {
        if (getline(inFile, line)) {
            auto instruction = Trace::createInstruction(line);
            this->halt = instruction->execute(this->cache);
            this->halt--;
            return true;
        }
    } else {
        this->halt--;
        return true;
    }
    
    // print out the end of execution time for the current core
    cout << "[CPU " + to_string(this->cpu_id) + "] Finished at cycle " << now << endl;
    inFile.close();
    return false;
}

CPU::~CPU() {
    if (inFile.is_open())
        inFile.close();
}
