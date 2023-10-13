#include "Cache.hpp"
#include "config.hpp"
#include <cmath>

using namespace std;

Cache::Cache(int cachesize, int associativity, int blocksize) {
    this->cachesize = cachesize;  // 1024
    this->associativity = associativity;  // 1
    this->blocksize = blocksize;  // 16
    this->set = this->cachesize / (associativity * blocksize);  // 64

    for (int i = 0; i < this->set; ++i)
        sets.push_back(CacheSet(this->associativity));
}

CacheAddress Cache::parseAddress(unsigned int address) const {
    int blockOffsetBits = std::log2(this->blocksize);
    int setIndexBits = std::log2(this->set);
    unsigned int blockOffsetMask = (1 << blockOffsetBits) - 1;
    unsigned int setIndexMask = ((1 << setIndexBits) - 1) << blockOffsetBits;

    CacheAddress result;
    result.blockOffset = address & blockOffsetMask;
    result.setIndex = (address & setIndexMask) >> blockOffsetBits;
    result.tag = address >> (blockOffsetBits + setIndexBits);
    return result;
}

// event triggered: PrRd
unsigned int Cache::read_addr(unsigned int address) {
    CacheAddress parse = this->parseAddress(address);
    if (this->sets[parse.setIndex].is_hit(parse.tag)) {
        // read hit, priority of the visited line has been boosted
        return TimeConfig::CacheHit;
    } else {
        return TimeConfig::CacheHit + this->sets[parse.setIndex].load_line(parse.tag);
        // read miss, ask other cpu cores, load the cache line from memory or other caches
        
    }
}

unsigned int Cache::write_addr(unsigned int address) {
    CacheAddress parse = this->parseAddress(address);
    if (this->sets[parse.setIndex].is_hit(parse.tag, true)) {
        // write hit, don't forget to set the dirty bit
        return TimeConfig::CacheHit;
    } else {
        // write miss, apply write allocate policy
        return TimeConfig::CacheHit + this->sets[parse.setIndex].load_line(parse.tag, true);
    }
}

unsigned int Cache::detect_addr(unsigned int address) const {
    CacheAddress parse = this->parseAddress(address);
    return this->sets[parse.setIndex].is_hit_readonly(parse.tag);
}
