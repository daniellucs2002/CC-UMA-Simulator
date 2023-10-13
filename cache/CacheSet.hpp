#include "CacheLine.hpp"
#include <vector>
#include <deque>

class Cache;

class CacheSet {
    friend class Cache;
private:
    int associativity;
    std::vector<CacheLine> lines;

    // maintain the time order of valid cache lines
    std::deque<int> idx;

    bool is_full() const;

public:
    CacheSet(int assoc);
    // write hit or miss needs to set the dirty bit
    bool is_hit(unsigned int tag, bool isWrite = false);
    // doesn't change the state of the system, return cycles need to wait
    int is_hit_readonly(unsigned int tag) const;
    int load_line(unsigned int tag, bool isWrite = false);
};
