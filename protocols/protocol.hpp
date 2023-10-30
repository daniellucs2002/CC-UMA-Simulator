#pragma once
#include "config.hpp"
#include "cache/CacheLine.hpp"
#include <vector>
#include <memory>
#include <map>

struct Message;

class CacheCoherenceProtocol {
public:
    std::map<int, State*> intToStringMap;

    // @ret: time in cycles to stay in the bus
    // @msg: message being processed
    // @flags: record the distribution of cached copies
    // @caches: pointer to the cache line that holds a copy of the data
    virtual int ProcessMsg(struct Message msg, std::vector<bool>& flags, std::vector<CacheLine*> caches) = 0;

    virtual int identify() const = 0;
};
