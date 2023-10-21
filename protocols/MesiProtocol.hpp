#pragma once
#include "protocol.hpp"
#include "config.hpp"
#include <cassert>
#include <iostream>
#include <cmath>

struct Message;

class MESICacheCoherenceProtocol : public CacheCoherenceProtocol {
public:
    int ProcessMsg(struct Message msg, std::vector<bool>& flags, std::vector<std::shared_ptr<CacheLine>> caches) {
        int cnt = 0;  // non-zeros
        for (int i = 0; i < cpunums; ++i)
            if (flags[i])
                cnt++;
        bool flag = false;  // modified exists or not
        for (int i = 0; i < cpunums; ++i)
            if (flags[i] && caches[i]->getState() == ModifiedState::getInstance()) {
                assert(!flag);  // at most one modified state
                flag = true;
            }
        if (!flags[msg.senderId]) {  // start from Invalid State
            if (msg.type == WRITE_REQ) {

            } else {  // READ_REQ
                if (cnt == 0) {

                } else {

                }
            }
        } else {  // Shared/Exclusive to Modified
            assert(flags[msg.senderId]);
            if (cnt == 1) {

            } else {

            }
            return 0;
        }
        return TimeConfig::LoadBlockFromMem;
    }
};
