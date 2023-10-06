#pragma once

struct CacheAddress {
    unsigned int tag;
    unsigned int setIndex;
    unsigned int blockOffset;
};

class Cache {
private:
    int cachesize;
    int associativity;
    int blocksize;
    int set;  // set number

    CacheAddress parseAddress(unsigned int address);

public:
    Cache(int cachesize, int associativity, int blocksize);
};
