#pragma once
#include "config.hpp"
#include "cache/CacheLine.hpp"
#include <vector>
#include <memory>

struct Message;

class CacheCoherenceProtocol {
public:
    // @ret: time in cycles to stay in the bus
    // @msg: message being processed
    // @flags: record the distribution of cached copies
    // @caches: pointer to the cache line that holds a copy of the data
    virtual int ProcessMsg(struct Message msg, std::vector<bool>& flags, std::vector<std::shared_ptr<CacheLine>> caches) = 0;
};
