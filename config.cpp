#include "config.hpp"

int cpunums = 4;
std::unordered_set<CacheAddress> insts;

std::shared_ptr<CacheCoherenceProtocol> protocol;

namespace CacheConfig {
    int cachesize = 1024;
    int associativity = 1;
    int blocksize = 16;
    int wordsize = 4;  // for every memory reference
}

std::string root_path = "/home/lujunchen/CC-UMA-Simulator/data/";

namespace TimeConfig {
    int CacheHit = 1;
    int LoadBlockFromMem = 100;
    int WriteBackMem = 100;
    int BusUpdate = 2;
}

std::vector<std::shared_ptr<Statistics>> cpu_stats;
