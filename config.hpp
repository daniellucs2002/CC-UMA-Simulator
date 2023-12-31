#pragma once
#include <string>
#include <memory>
#include <unordered_set>
#include "protocols/protocol.hpp"
#include "stats.hpp"

extern int cpunums;

extern std::shared_ptr<CacheCoherenceProtocol> protocol;

namespace CacheConfig {
    extern int cachesize;
    extern int associativity;
    extern int blocksize;
    extern int wordsize;
}

extern std::string root_path;

namespace TimeConfig {
    extern int CacheHit;
    extern int LoadBlockFromMem;
    extern int WriteBackMem;
    extern int BusUpdate;
}

struct CacheAddress {
    unsigned int tag;
    unsigned int setIndex;

    bool operator==(const CacheAddress& other) const {
        return this->tag == other.tag && this->setIndex == other.setIndex;
    }
};

namespace std {
    template <>
    struct hash<CacheAddress> {
        std::size_t operator()(const CacheAddress& address) const {
            size_t hash = 17;
            hash = hash * 31 + std::hash<unsigned int>{}(address.tag);
            hash = hash * 31 + std::hash<unsigned int>{}(address.setIndex);
            return hash;
        }
    };
}

enum MessageType {
    READ_REQ,
    WRITE_REQ,  // for MESI protocol
};

struct Message {
    int senderId;
    int stayInBus;
    CacheAddress address;
    MessageType type;
};

extern std::unordered_set<CacheAddress> insts;

extern std::vector<std::shared_ptr<Statistics>> cpu_stats;
