#include "Bus.hpp"
#include "config.hpp"

void Bus::registerCache(std::shared_ptr<CacheController> cache) {
    caches.push_back(cache);
}

void Bus::sendRequest(const Message& msg) {
    requestMessages.push(msg);
}

void Bus::sendReply(const Message& msg) {
    replyMessages.push(msg);
}

void Bus::propagateRequests() {
    std::vector<bool> flags(cpunums);  // caches hold a copy or not
    std::vector<CacheLine*> caches(cpunums);

    while (!requestMessages.empty()) {
        Message msg = requestMessages.front();
        requestMessages.pop();

        for (int i = 0; i < cpunums; ++i) {
            flags[i] = false;
            caches[i] = nullptr;
        }

        // new request, stayInBus can decide upon status of other caches
        // change state of caches and decide the latency till reply
        if (msg.stayInBus == -1) {
            msg.stayInBus = TimeConfig::CacheHit;
            // msg.stayInBus = TimeConfig::LoadBlockFromMem + TimeConfig::CacheHit;
            // step1: collect information about caches that hold a copy
            for (int i = 0; i < cpunums; ++i) {
                std::shared_ptr<Cache> ptr = this->caches[i]->cache;
                caches[i] = ptr->getSets()[msg.address.setIndex].is_hit_msg(msg.address.tag);
                if (caches[i] != nullptr)
                    flags[i] = true;
            }

            // step2: decide new states and apply changes

            // step3: calculate time in bus before sending reply
            
            msg.stayInBus += protocol->ProcessMsg(msg, flags, caches);
        }
        msg.stayInBus--;
        this->sendReply(msg);

        // for (std::shared_ptr<CacheController> cache : caches)
        //     if (cache->getId() != msg.senderId)
        //         cache->receiveRequest(msg);
    }
}

void Bus::propagateReplies() {
    while (!replyMessages.empty()) {
        Message msg = replyMessages.front();
        replyMessages.pop();

        if (msg.stayInBus != 0) {
            this->sendRequest(msg);
        } else {
            // notify the cache that the block has been loaded from memory
            for (std::shared_ptr<CacheController> cache : caches)
                if (cache->getId() == msg.senderId)
                    cache->receiveReply(msg);
        }
    }
}
