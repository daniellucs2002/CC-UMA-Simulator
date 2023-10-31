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

    int identify() const override {return 2;}

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

        bool has_SharedModified = false;
        for (int i = 0; i < cpunums; ++i)
            if (flags[i] && caches[i]->getState() == DragonSharedModified::getInstance()) {
                assert(!has_SharedModified && !has_modified);  // no modified allowed
                has_SharedModified = true;
            }

        // start the logic of Dragon state transition
        if (!flags[msg.senderId]) {  // start from Invalid State

            // transition initiates from Invalid, which means applying changes has to be delayed
            assert(intToStringMap.find(msg.senderId) == intToStringMap.end());

            // on processor write miss
            if (msg.type == WRITE_REQ) {  // BusUpdate is always associated with processor write
                if (cnt == 0) {
                    intToStringMap.insert(std::make_pair(msg.senderId, DragonModified::getInstance()));
                } else if (cnt == 1) {

                    // find the cache that holds the copy and UPDATE
                    // possibilities: DragonModified, DragonExclusive, DragonSharedClean, DragonSharedModified
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            if (has_modified || has_SharedModified) {
                                assert(caches[i]->getState() == DragonModified::getInstance() ||
                                    caches[i]->getState() == DragonSharedModified::getInstance());
                                assert(caches[i]->getDirty());
                                caches[i]->setDirty(false);  // to shared clean
                            }
                            assert(!caches[i]->getDirty());
                            caches[i]->setState(DragonSharedClean::getInstance());
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, DragonSharedModified::getInstance()));

                    // sending a cache block with N words to another cache takes 2N cycles
                    // BusUpdate takes only two cycles; first send the whole block, then bus update
                    return CacheConfig::blocksize / 2 + TimeConfig::BusUpdate;
                } else {
                    assert(!has_modified);  // Modified cannot exist
                    for (int i = 0; i < cpunums; ++i)
                        if (flags[i]) {
                            cnt--;
                            assert(caches[i]->getState() == DragonSharedClean::getInstance() ||
                                caches[i]->getState() == DragonSharedModified::getInstance());
                            if (caches[i]->getState() == DragonSharedClean::getInstance())
                                assert(!caches[i]->getDirty());
                            caches[i]->setState(DragonSharedClean::getInstance());
                            if (caches[i]->getState() == DragonSharedModified::getInstance()) {
                                assert(caches[i]->getDirty());
                                caches[i]->setDirty(false);  // to shared clean
                            }
                            assert(!caches[i]->getDirty());
                        }
                    assert(cnt == 0);
                    intToStringMap.insert(std::make_pair(msg.senderId, DragonSharedModified::getInstance()));
                    return CacheConfig::blocksize / 2 + TimeConfig::BusUpdate;
                }
            } else {  // READ_REQ
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
                    } else if (has_SharedModified) {  // O
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
                            if (caches[i]->getState() == DragonSharedClean::getInstance())
                                assert(!caches[i]->getDirty());
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

                if (cnt == 1) {  // no need to update, transfer to DragonModified
                    assert(!caches[msg.senderId]->getDirty());
                    caches[msg.senderId]->setState(DragonModified::getInstance());
                } else {  // SharedClean to SharedModified, others need to be updated and transfer to SharedClean
                    for (int i = 0; i < cpunums; ++i)
                        if (i != msg.senderId && flags[i]) {
                            assert(caches[i]->getState() == DragonSharedClean::getInstance() ||
                                caches[i]->getState() == DragonSharedModified::getInstance());
                            if (caches[i]->getState() == DragonSharedModified::getInstance()) {
                                assert(has_SharedModified && caches[i]->getDirty());
                                caches[i]->setDirty(false);  // to shared clean
                                caches[i]->setState(DragonSharedClean::getInstance());
                            } else {
                                assert(!caches[i]->getDirty());
                            }
                        }
                    caches[msg.senderId]->setState(DragonSharedModified::getInstance());
                    assert(!caches[msg.senderId]->getDirty());
                    return TimeConfig::BusUpdate;
                }
            } else if (caches[msg.senderId]->getState() == DragonSharedModified::getInstance()) {

                if (cnt == 1) {
                    assert(caches[msg.senderId]->getDirty());
                    caches[msg.senderId]->setState(DragonModified::getInstance());
                } else {  // other copies are all in SharedClean state
                    for (int i = 0; i < cpunums; ++i)
                        if (i != msg.senderId && flags[i]) {
                            assert(caches[i]->getState() == DragonSharedClean::getInstance());
                            assert(!caches[i]->getDirty());
                        }
                    return TimeConfig::BusUpdate;
                }
            } else if (caches[msg.senderId]->getState() == DragonModified::getInstance()) {
                // do nothing, fall through
            } else {
                assert(false);  // never reach here
            }
            return 0;
        }
        return TimeConfig::LoadBlockFromMem;
    }
};
