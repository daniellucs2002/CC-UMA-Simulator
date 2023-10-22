#pragma once
#include "protocol.hpp"
#include "config.hpp"
#include <cassert>
#include <iostream>
#include <cmath>

struct Message;

class MESICacheCoherenceProtocol : public CacheCoherenceProtocol {
public:
    // state to be set when the cache line has been allocated
    // std::map<int, State*> intToStringMap;

    int ProcessMsg(struct Message msg, std::vector<bool>& flags, std::vector<CacheLine*> caches) {

        // how many caches in the system that hold a data copy
        int cnt = 0;  // non-zeros
        for (int i = 0; i < cpunums; ++i)
            if (flags[i])
                cnt++;
        
        // whether or not Modified state exists in the system
        bool flag = false;  // modified exists or not
        for (int i = 0; i < cpunums; ++i)
            if (flags[i] && caches[i]->getState() == ModifiedState::getInstance()) {
                assert(!flag && cnt == 1);  // at most one modified state
                flag = true;
            }

        // start the logic of MESI state transition
        if (!flags[msg.senderId]) {  // start from Invalid State

            // transition initiates from Invalid, which means applying changes has to be delayed
            assert(intToStringMap.find(msg.senderId) == intToStringMap.end());

            // on processor write miss
            if (msg.type == WRITE_REQ) {
                if (cnt == 0) {
                    intToStringMap.insert(std::make_pair(msg.senderId, ModifiedState::getInstance()));
                } else if (cnt == 1) {

                    // invalidate the cache that holds the copy
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            if (flag)
                                assert(caches[i]->getState() == ModifiedState::getInstance());
                            caches[i]->setState(InvalidState::getInstance());
                            caches[i]->setValid(false);
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, ModifiedState::getInstance()));

                    // sending a cache block with N words to another cache takes 2N cycles
                    return CacheConfig::blocksize / 2;
                } else {
                    assert(!flag);  // Modified cannot exist
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            assert(caches[i]->getState() == SharedState::getInstance());
                            caches[i]->setState(InvalidState::getInstance());
                            caches[i]->setValid(false);
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, ModifiedState::getInstance()));
                    return CacheConfig::blocksize / 2;
                }
            } else {  // READ_REQ
                if (cnt == 0) {  // Invalid to Exclusive, state cannot be set currently
                    intToStringMap.insert(std::make_pair(msg.senderId, ExclusiveState::getInstance()));
                } else if (cnt == 1) {
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            if (flag)
                                assert(caches[i]->getState() == ModifiedState::getInstance());
                            caches[i]->setState(SharedState::getInstance());
                            // if Modified, reset the dirty bit to false
                            if (flag) {
                                assert(caches[i]->getDirty());
                                caches[i]->setDirty(false);
                            }
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, SharedState::getInstance()));

                    // Modified to Shared needs writing back to memory, so max(..., ...)
                    if (flag)
                        return max(CacheConfig::blocksize / 2, TimeConfig::WriteBackMem);
                    else
                        return CacheConfig::blocksize / 2;
                } else {
                    assert(!flag);  // Modified cannot exist
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            assert(caches[i]->getState() == SharedState::getInstance());
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, SharedState::getInstance()));
                    return CacheConfig::blocksize / 2;
                }
            }
        } else {
            assert(flags[msg.senderId]);
            assert(msg.type == WRITE_REQ);
            if (cnt == 1) {  // Exclusive to Modified, PrWr/-
                assert(caches[msg.senderId]->getState() == ExclusiveState::getInstance() || caches[msg.senderId]->getState() == SharedState::getInstance());
                caches[msg.senderId]->setState(ModifiedState::getInstance());
            } else {  // Shared to Modified, PrWr/BusRdX
                assert(caches[msg.senderId]->getState() == SharedState::getInstance());
                caches[msg.senderId]->setState(ModifiedState::getInstance());
                // also invalidate all the other cache lines
                for (int i = 0; i < cpunums; ++i)
                    if (i != msg.senderId && flags[i]) {
                        assert(caches[i]->getState() == SharedState::getInstance());
                        caches[i]->setState(InvalidState::getInstance());
                        caches[i]->setValid(false);
                    }
            }
            return 0;
        }
        return TimeConfig::LoadBlockFromMem;
    }
};
