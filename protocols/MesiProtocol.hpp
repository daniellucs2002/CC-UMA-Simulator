#pragma once
#include "protocol.hpp"
#include "config.hpp"

struct Message;

class MESICacheCoherenceProtocol : public CacheCoherenceProtocol {
public:
    int ProcessMsg(struct Message msg, std::vector<bool>& flags, std::vector<std::shared_ptr<CacheLine>> caches) {
        return TimeConfig::LoadBlockFromMem;
    }
};
