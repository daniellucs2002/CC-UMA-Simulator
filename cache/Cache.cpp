#include "Cache.hpp"

using namespace std;

Cache::Cache(int cachesize, int associativity, int blocksize) {
    this->cachesize = cachesize;  // 1024
    this->associativity = associativity;  // 1
    this->blocksize = blocksize;  // 16
    this->set = this->cachesize / (associativity * blocksize);  // 64

    for (int i = 0; i < this->set; ++i)
        sets.push_back(CacheSet(this->associativity));
}
