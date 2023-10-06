#include <vector>
#include "CacheLine.hpp"

class CacheSet {
private:
    int associativity;

    std::vector<CacheLine> lines;

public:
    CacheSet(int associativity);
};
