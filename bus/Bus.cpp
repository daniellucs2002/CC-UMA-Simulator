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
    while (!requestMessages.empty()) {
        Message msg = requestMessages.front();
        requestMessages.pop();

        // new request, stayInBus can decide upon status of other caches
        if (msg.stayInBus == -1)
            msg.stayInBus = TimeConfig::LoadBlockFromMem + TimeConfig::CacheHit;
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
