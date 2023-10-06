#pragma once
#include <vector>
#include <iostream>
#include "CacheSet.hpp"

class Cache {
private:
    int cachesize;
    int associativity;
    int blocksize;
    int set;  // set number

    std::vector<CacheSet> sets;

public:
    Cache(int cachesize, int associativity, int blocksize);
};
