#pragma once
#include "protocol.hpp"
#include "config.hpp"
#include "states/MoesiState.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

struct Message;

class MOESICacheCoherenceProtocol : public CacheCoherenceProtocol {
public:
    // state to be set when the cache line has been allocated
    // std::map<int, State*> intToStringMap;

    int identify() const override {return 1;}

    int ProcessMsg(struct Message msg, std::vector<bool>& flags, std::vector<CacheLine*> caches) {

        // how many caches in the system that hold a data copy
        int cnt = 0;  // non-zeros
        for (int i = 0; i < cpunums; ++i)
            if (flags[i])
                cnt++;
        
        // whether or not Modified state exists in the system
        bool has_modified = false;  // modified exists or not
        for (int i = 0; i < cpunums; ++i)
            if (flags[i] && caches[i]->getState() == ModifiedState::getInstance()) {
                assert(!has_modified && cnt == 1);  // at most one modified state
                has_modified = true;
            }

        bool has_owned = false;
        for (int i = 0; i < cpunums; ++i)
            if (flags[i] && caches[i]->getState() == OwnedState::getInstance()) {
                assert(!has_owned && !has_modified);  // no modified allowed
                has_owned = true;
            }

        // start the logic of MOESI state transition
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
                            if (has_modified)
                                assert(caches[i]->getState() == ModifiedState::getInstance());
                            if (has_owned)
                                assert(caches[i]->getState() == OwnedState::getInstance());
                            caches[i]->setState(InvalidState::getInstance());
                            caches[i]->setValid(false);
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, ModifiedState::getInstance()));

                    // sending a cache block with N words to another cache takes 2N cycles
                    return CacheConfig::blocksize / 2;
                } else {
                    assert(!has_modified);  // Modified cannot exist
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            assert(caches[i]->getState() == SharedState::getInstance() ||
                                caches[i]->getState() == OwnedState::getInstance());
                            caches[i]->setState(InvalidState::getInstance());
                            caches[i]->setValid(false);
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, ModifiedState::getInstance()));
                    return CacheConfig::blocksize / 2;
                }
            } else {  // READ_REQ, which can reveal the benefits of MOESI
                if (cnt == 0) {  // Invalid to Exclusive, state cannot be set currently
                    intToStringMap.insert(std::make_pair(msg.senderId, ExclusiveState::getInstance()));
                } else if (cnt == 1) {
                    // four cases: M O E S
                    if (has_modified) {  // M
                        for (int i = 0; i < cpunums; ++i)
                            if (flags[i] && caches[i]->getState() == ModifiedState::getInstance()) {
                                caches[i]->setState(OwnedState::getInstance());
                                assert(caches[i]->getDirty());  // the dirty bit should be kept
                            }
                        intToStringMap.insert(std::make_pair(msg.senderId, SharedState::getInstance()));
                        return CacheConfig::blocksize / 2;  // improvement: no writing back in this case
                    } else if (has_owned) {  // O
                        intToStringMap.insert(std::make_pair(msg.senderId, SharedState::getInstance()));
                        return CacheConfig::blocksize / 2;
                    } else {  // E and S
                        for (int i = 0; i < cpunums; ++i)
                            if (flags[i]) {
                                cnt--;
                                assert(caches[i]->getState() == ExclusiveState::getInstance() ||
                                    caches[i]->getState() == SharedState::getInstance());
                                // set the state to shared anyway
                                caches[i]->setState(SharedState::getInstance());
                                assert(!caches[i]->getDirty());
                            }
                        assert(cnt == 0);
                        intToStringMap.insert(std::make_pair(msg.senderId, SharedState::getInstance()));
                        return CacheConfig::blocksize / 2;
                    }
                } else {  // only shared, or shared + owned
                    assert(!has_modified);  // Modified cannot exist
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            assert(caches[i]->getState() == SharedState::getInstance() ||
                                caches[i]->getState() == OwnedState::getInstance());
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, SharedState::getInstance()));
                    return CacheConfig::blocksize / 2;
                }
            }
        } else {
            assert(flags[msg.senderId]);
            assert(msg.type == WRITE_REQ);
            if (caches[msg.senderId]->getState() == ExclusiveState::getInstance()) {
                // Exclusive to Modified, PrWr/-
                assert(cnt == 1);
                caches[msg.senderId]->setState(ModifiedState::getInstance());
            } else if (caches[msg.senderId]->getState() == SharedState::getInstance()) {
                caches[msg.senderId]->setState(ModifiedState::getInstance());
                // also invalidate all the other cache lines
                for (int i = 0; i < cpunums; ++i)
                    if (i != msg.senderId && flags[i]) {
                        assert(caches[i]->getState() == SharedState::getInstance() ||
                            caches[i]->getState() == OwnedState::getInstance());
                        if (caches[i]->getState() == OwnedState::getInstance())
                            assert(has_owned && caches[i]->getDirty());
                        caches[i]->setState(InvalidState::getInstance());
                        caches[i]->setValid(false);
                    }
                // attention: OwnedState is responsible for writing dirty block back into memory
                // if (has_owned)
                //     return TimeConfig::WriteBackMem;
            } else if (caches[msg.senderId]->getState() == OwnedState::getInstance()) {
                caches[msg.senderId]->setState(ModifiedState::getInstance());
                assert(caches[msg.senderId]->getDirty());
                // also invalidate all the other cache lines
                for (int i = 0; i < cpunums; ++i)
                    if (i != msg.senderId && flags[i]) {
                        assert(caches[i]->getState() == SharedState::getInstance());
                        caches[i]->setState(InvalidState::getInstance());
                        caches[i]->setValid(false);
                    }
            } else if (caches[msg.senderId]->getState() == ModifiedState::getInstance()) {
                // do nothing, fall through
            } else {
                assert(false);  // never reach here
            }
            return 0;
        }
        return TimeConfig::LoadBlockFromMem;
    }
};
