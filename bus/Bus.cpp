#include "Bus.hpp"

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

        for (std::shared_ptr<CacheController> cache : caches)
            if (cache->getId() != msg.senderId)
                cache->receiveRequest(msg);
    }
}

void Bus::propagateReplies() {
    while (!replyMessages.empty()) {
        Message msg = replyMessages.front();
        replyMessages.pop();

        for (std::shared_ptr<CacheController> cache : caches)
            if (cache->getId() == msg.senderId)
                cache->receiveReply(msg);
    }
}
