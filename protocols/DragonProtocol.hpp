#pragma once
#include "protocol.hpp"
#include "config.hpp"
#include "states/DragonState.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

struct Message;

class DragonCacheCoherenceProtocol : public CacheCoherenceProtocol {
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
        bool has_modified = false;  // modified exists or not
        for (int i = 0; i < cpunums; ++i)
            if (flags[i] && caches[i]->getState() == DragonModified::getInstance()) {
                assert(!has_modified && cnt == 1);  // at most one modified state
                has_modified = true;
            }

        bool has_owned = false;
        for (int i = 0; i < cpunums; ++i)
            if (flags[i] && caches[i]->getState() == DragonSharedModified::getInstance()) {
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
                    intToStringMap.insert(std::make_pair(msg.senderId, DragonModified::getInstance()));
                } else if (cnt == 1) {

                    // invalidate the cache that holds the copy
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            if (has_modified)
                                assert(caches[i]->getState() == DragonModified::getInstance());
                            if (has_owned)
                                assert(caches[i]->getState() == DragonSharedModified::getInstance());
                            caches[i]->setState(DragonInvalid::getInstance());
                            caches[i]->setValid(false);
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, DragonModified::getInstance()));

                    // sending a cache block with N words to another cache takes 2N cycles
                    return CacheConfig::blocksize / 2;
                } else {
                    assert(!has_modified);  // Modified cannot exist
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            assert(caches[i]->getState() == DragonSharedClean::getInstance() ||
                                caches[i]->getState() == DragonSharedModified::getInstance());
                            caches[i]->setState(DragonInvalid::getInstance());
                            caches[i]->setValid(false);
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, DragonModified::getInstance()));
                    return CacheConfig::blocksize / 2;
                }
            } else {  // READ_REQ, which can reveal the benefits of MOESI
                if (cnt == 0) {  // Invalid to Exclusive, state cannot be set currently
                    intToStringMap.insert(std::make_pair(msg.senderId, DragonExclusive::getInstance()));
                } else if (cnt == 1) {
                    // four cases: M O E S
                    if (has_modified) {  // M
                        for (int i = 0; i < cpunums; ++i)
                            if (flags[i] && caches[i]->getState() == DragonModified::getInstance()) {
                                caches[i]->setState(DragonSharedModified::getInstance());
                                assert(caches[i]->getDirty());  // the dirty bit should be kept
                            }
                        intToStringMap.insert(std::make_pair(msg.senderId, DragonSharedClean::getInstance()));
                        return CacheConfig::blocksize / 2;  // improvement: no writing back in this case
                    } else if (has_owned) {  // O
                        intToStringMap.insert(std::make_pair(msg.senderId, DragonSharedClean::getInstance()));
                        return CacheConfig::blocksize / 2;
                    } else {  // E and S
                        for (int i = 0; i < cpunums; ++i)
                            if (flags[i]) {
                                cnt--;
                                assert(caches[i]->getState() == DragonExclusive::getInstance() ||
                                    caches[i]->getState() == DragonSharedClean::getInstance());
                                // set the state to shared anyway
                                caches[i]->setState(DragonSharedClean::getInstance());
                                assert(!caches[i]->getDirty());
                            }
                        assert(cnt == 0);
                        intToStringMap.insert(std::make_pair(msg.senderId, DragonSharedClean::getInstance()));
                        return CacheConfig::blocksize / 2;
                    }
                } else {  // only shared, or shared + owned
                    assert(!has_modified);  // Modified cannot exist
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            assert(caches[i]->getState() == DragonSharedClean::getInstance() ||
                                caches[i]->getState() == DragonSharedModified::getInstance());
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, DragonSharedClean::getInstance()));
                    return CacheConfig::blocksize / 2;
                }
            }
        } else {
            assert(flags[msg.senderId]);
            assert(msg.type == WRITE_REQ);
            if (caches[msg.senderId]->getState() == DragonExclusive::getInstance()) {
                // Exclusive to Modified, PrWr/-
                assert(cnt == 1);
                caches[msg.senderId]->setState(DragonModified::getInstance());
            } else if (caches[msg.senderId]->getState() == DragonSharedClean::getInstance()) {
                caches[msg.senderId]->setState(DragonModified::getInstance());
                // also invalidate all the other cache lines
                for (int i = 0; i < cpunums; ++i)
                    if (i != msg.senderId && flags[i]) {
                        assert(caches[i]->getState() == DragonSharedClean::getInstance() ||
                            caches[i]->getState() == DragonSharedModified::getInstance());
                        if (caches[i]->getState() == DragonSharedModified::getInstance())
                            assert(has_owned && caches[i]->getDirty());
                        caches[i]->setState(DragonInvalid::getInstance());
                        caches[i]->setValid(false);
                    }
                // attention: DragonSharedModified is responsible for writing dirty block back into memory
                // if (has_owned)
                //     return TimeConfig::WriteBackMem;
            } else if (caches[msg.senderId]->getState() == DragonSharedModified::getInstance()) {
                caches[msg.senderId]->setState(DragonModified::getInstance());
                assert(caches[msg.senderId]->getDirty());
                // also invalidate all the other cache lines
                for (int i = 0; i < cpunums; ++i)
                    if (i != msg.senderId && flags[i]) {
                        assert(caches[i]->getState() == DragonSharedClean::getInstance());
                        caches[i]->setState(DragonInvalid::getInstance());
                        caches[i]->setValid(false);
                    }
            } else {
                assert(false);  // never reach here
            }
            return 0;
        }
        return TimeConfig::LoadBlockFromMem;
    }
};
