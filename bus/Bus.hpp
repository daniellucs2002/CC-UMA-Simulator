#pragma once
#include <vector>
#include <queue>
#include <memory>
#include "cache/CacheController.hpp"
#include "cache/Cache.hpp"
#include "config.hpp"

class CacheController;

class Bus {
    std::vector<std::shared_ptr<CacheController>> caches;
    std::queue<Message> requestMessages;
    std::queue<Message> replyMessages;

public:
    void registerCache(std::shared_ptr<CacheController> cache);
    void sendRequest(const Message& msg);
    void sendReply(const Message& msg);
    void propagateRequests();
    void propagateReplies();
};
