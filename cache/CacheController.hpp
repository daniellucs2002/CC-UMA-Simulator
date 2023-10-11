#pragma once
#include "bus/Bus.hpp"
#include "config.hpp"

class Cache;
class Bus;

class CacheController : public std::enable_shared_from_this<CacheController> {
    int id;
    std::shared_ptr<Bus> bus;
    std::shared_ptr<Cache> cache;

public:
    CacheController(int i, std::shared_ptr<Bus> b, std::shared_ptr<Cache> c);
    int getId() const;
    void sendRequest(const Message& msg);
    void receiveRequest(const Message& msg);
    void sendReply(const Message& msg);
    void receiveReply(const Message& msg);
};
