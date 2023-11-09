#include "cpu/CPU.hpp"
#include "config.hpp"
#include "traces/Trace.hpp"

extern string root_path;

CPU::CPU(int id, string filename) {
    this->cpu_id = id;
    this->inst = nullptr;
    this->halt = 0;  // default, current instruction lasts for one cycle
    this->filename = root_path + filename + to_string(id) + ".data";
    // cout << "Constructing CPU " << this->cpu_id << "..." << endl;
    this->inFile.open(this->filename);
    assert(this->inFile.is_open());

    // create the unique_ptr to L1 data cache
    this->cache = std::make_shared<Cache>(CacheConfig::cachesize,
        CacheConfig::associativity, CacheConfig::blocksize, this->cpu_id);
}

bool CPU::update(unsigned long now) {
    // read a line from the trace file until it finishes
    string line;
    if (!this->inFile.is_open())
        return false;
    if (this->halt == 0) {
        if (this->inst != nullptr) {
            // an instruction that hasn't been processed
            CacheAddress parse = this->cache->parseAddress(this->inst->getVal());
            if (insts.find(parse) != insts.end()) {
                // which means the CacheAddress is still conflicting
                // cpu_stats[this->cpu_id]->Increment("idle_cycles");
                return true;
            } else {
                // issue the stored instruction before reading another line from file
                insts.insert(parse);
                this->halt = this->inst->detect(this->cache);
                --this->halt;
                // cpu_stats[this->cpu_id]->Increment("idle_cycles");
                if (this->halt == 0 && this->inst.get() != nullptr) {
                    this->inst->execute(this->cache);
                    insts.erase(parse);
                    this->inst = nullptr;
                }
                return true;
            }
        }
        if (getline(inFile, line)) {
            auto instruction = Trace::createInstruction(line);
            if (instruction->identify() == 0 || instruction->identify() == 1) {
                if (instruction->identify() == 0) {
                    cpu_stats[this->cpu_id]->Increment("load_number");
                } else {
                    cpu_stats[this->cpu_id]->Increment("store_number");
                }
                CacheAddress parse = this->cache->parseAddress(instruction->getVal());
                if (insts.find(parse) != insts.end()) {
                    // in case of conflicting CacheAddress, halt the current core and store the instruction
                    this->halt = 0;
                    this->inst = instruction;
                    // cpu_stats[this->cpu_id]->Increment("idle_cycles");
                    return true;
                }
                insts.insert(parse);
            } else {
                cpu_stats[this->cpu_id]->AddMany("compute_cycles", instruction->getVal());
            }
            this->halt = instruction->detect(this->cache);
            // record the instruction into the object for later execution
            this->inst = instruction;
            --this->halt;
            // cpu_stats[this->cpu_id]->Increment("idle_cycles");
            if (this->halt == 0 && this->inst.get() != nullptr) {
                this->inst->execute(this->cache);
                if (this->inst->identify() == 0 || this->inst->identify() == 1) {
                    CacheAddress parse = this->cache->parseAddress(this->inst->getVal());
                    assert(insts.find(parse) != insts.end());  // must exist
                    insts.erase(parse);
                }
                this->inst = nullptr;
            }
            return true;
        }
    } else {
        --this->halt;
        // cpu_stats[this->cpu_id]->Increment("idle_cycles");
        if (this->halt == 0 && this->inst.get() != nullptr) {
            this->inst->execute(this->cache);
            if (this->inst->identify() == 0 || this->inst->identify() == 1) {
                CacheAddress parse = this->cache->parseAddress(this->inst->getVal());
                assert(insts.find(parse) != insts.end());  // must exist
                insts.erase(parse);
            }
            this->inst = nullptr;
        }
        return true;
    }
    
    // print out the end of execution time for the current core
    // cout << "[CPU " + to_string(this->cpu_id) + "] Finished at cycle " << now << endl;
    cpu_stats[this->cpu_id]->SetCount("sum_execution_time", now);
    inFile.close();
    return false;
}

CPU::~CPU() {
    if (inFile.is_open())
        inFile.close();
}
