#pragma once
#include <string>

extern int cpunums;

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
}

struct CacheAddress {
    unsigned int tag;
    unsigned int setIndex;
    unsigned int blockOffset;
};

enum MessageType {
    READ_REQUEST,
    READ_REPLY,
};

struct Message {
    int senderId;
    MessageType type;
    CacheAddress address;
    bool success;
};
