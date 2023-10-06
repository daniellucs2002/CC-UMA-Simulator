#include "CacheSet.hpp"

CacheSet::CacheSet(int associativity) {
    this->associativity = associativity;

    for (int i = 0; i < this->associativity; ++i)
        lines.push_back(CacheLine());
}
