#include "Cache.hpp"
#include <cmath>

using namespace std;

Cache::Cache(int cachesize, int associativity, int blocksize) {
    this->cachesize = cachesize;  // 1024
    this->associativity = associativity;  // 1
    this->blocksize = blocksize;  // 16
    this->set = this->cachesize / (associativity * blocksize);  // 64

}

CacheAddress Cache::parseAddress(unsigned int address) {
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
