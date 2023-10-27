#include "CacheSet.hpp"
#include "config.hpp"
#include "states/MesiState.hpp"
#include <algorithm>
#include <cassert>
#include <memory>

class CacheLine;

CacheSet::CacheSet(int assoc, int id) {
    this->id = id;
    this->associativity = assoc;
    for (int i = 0; i < this->associativity; ++i)
        this->lines.push_back(CacheLine());
    for (int i = 0; i < this->associativity; ++i)
        this->lines[i].setState(InvalidState::getInstance());
}

bool CacheSet::is_full() const {
    for (int i = 0; i < this->associativity; ++i)
        if (this->lines[i].is_valid == false)
            return false;
    return true;
}

bool CacheSet::is_hit(unsigned int tag, bool isWrite) {
    for (int i = 0; i < this->associativity; ++i)
        if (this->lines[i].is_valid && this->lines[i].tag == tag) {
            // read hit, boost the priority in the std::deque
            this->idx.erase(std::find(this->idx.begin(), this->idx.end(), i));
            this->idx.push_front(i);
            // set the dirty bit for write hit
            if (isWrite)
                this->lines[i].is_dirty = true;
            return true;
        }
    return false;
}

CacheLine* CacheSet::is_hit_msg(unsigned int tag) {
    for (int i = 0; i < this->associativity; ++i)
        if (this->lines[i].is_valid && this->lines[i].tag == tag)
            return &this->lines[i];
    return nullptr;
}

// define const functions when necessary
// write hit but is_dirty=false, we still need to send the message
bool CacheSet::is_hit_readonly(unsigned int tag, bool isWrite) const {
    for (int i = 0; i < this->associativity; ++i)
        if (this->lines[i].is_valid && this->lines[i].tag == tag) {
            if (isWrite && this->lines[i].is_dirty == false)
                return false;
            return true;
        }
    return false;
    // several possible cases in cache miss
    // if (!is_full()) {
    //     return TimeConfig::LoadBlockFromMem + TimeConfig::CacheHit;
    // } else {
    //     // similarly, only read operations
    //     int evict_idx = this->idx.back();
    //     bool writeback = this->lines[evict_idx].is_dirty;
    //     if (writeback)
    //         return TimeConfig::LoadBlockFromMem + TimeConfig::WriteBackMem + TimeConfig::CacheHit;
    //     else
    //         return TimeConfig::LoadBlockFromMem + TimeConfig::CacheHit;
    // }
}

bool CacheSet::need_write_back(unsigned int tag) const {
    if (is_full()) {
        int evict_idx = this->idx.back();
        return this->lines[evict_idx].is_dirty;
    }
    return false;
}

// on cache miss, read or write
int CacheSet::load_line(unsigned int tag, bool isWrite) {
    if (!is_full()) {  // pick an invalid line
        for (int i = 0; i < this->associativity; ++i)
            if (this->lines[i].is_valid == false) {
                assert(this->lines[i].getState() == InvalidState::getInstance());
                this->lines[i].is_valid = true;
                this->lines[i].tag = tag;
                if (isWrite)
                    this->lines[i].is_dirty = true;
                else
                    this->lines[i].is_dirty = false;
                // assume data has been loaded into cache from memory, no write back
                this->idx.push_front(i);
                if (protocol->intToStringMap.find(this->id) == protocol->intToStringMap.end()) {
                    // gotta find instructions to set new state
                    assert(false);
                } else {
                    this->lines[i].setState(protocol->intToStringMap[this->id]);
                    protocol->intToStringMap.erase(this->id);
                }
                return TimeConfig::LoadBlockFromMem;
            }
    } else {  // evict a line based on LRU policy
        int evict_idx = this->idx.back();
        // check the dirty bit to see "write back" or not
        bool writeback = this->lines[evict_idx].is_dirty;
        this->idx.pop_back();
        // evict old line and load new cache line from memory
        assert(this->lines[evict_idx].is_valid);
        this->lines[evict_idx].tag = tag;
        if (isWrite)
            this->lines[evict_idx].is_dirty = true;
        else
            this->lines[evict_idx].is_dirty = false;
        this->idx.push_front(evict_idx);
        if (protocol->intToStringMap.find(this->id) == protocol->intToStringMap.end()) {
            assert(false);
        } else {
            this->lines[evict_idx].setState(protocol->intToStringMap[this->id]);
            protocol->intToStringMap.erase(this->id);
        }
        if (writeback)
            return TimeConfig::LoadBlockFromMem + TimeConfig::WriteBackMem;
        else
            return TimeConfig::LoadBlockFromMem;
    }
    assert(false);
    return 0;
}
