#pragma once

#include <vector>
#include "CacheSet.hpp"

struct CacheAddress {
    unsigned int tag;
    unsigned int setIndex;
    unsigned int blockOffset;
};

// This is a blocking cache!
class Cache {
private:
    int cachesize;
    int associativity;
    int blocksize;
    int set;  // set number

    std::vector<CacheSet> sets;

    CacheAddress parseAddress(unsigned int address);

public:
    Cache(int cachesize, int associativity, int blocksize);

    // input: address to be visited
    // output: cycles needed for completion
    unsigned int read_addr(unsigned int address);

    // input: address to be written to
    // ouput: cycles it takes to write address
    unsigned int write_addr(unsigned int address);
};
