#include "CacheLine.hpp"
#include <vector>
#include <deque>
#include <memory>

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

    // doesn't change the state of the system, just peek
    bool is_hit_readonly(unsigned int tag, bool isWrite = false) const;  // cache hit or not
    bool need_write_back(unsigned int tag) const;  // write back or just discard
    
    int load_line(unsigned int tag, bool isWrite = false);

    // used to evaluate global situation
    std::shared_ptr<CacheLine> is_hit_msg(unsigned int tag) const;
};
