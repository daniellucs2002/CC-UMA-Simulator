#include "CacheController.hpp"

CacheController::CacheController(int i, std::shared_ptr<Bus> b, std::shared_ptr<Cache> c)
    : id(i), bus(b), cache(c)
{
    
}

int CacheController::getId() const {
    return id;
}

void CacheController::sendRequest(const Message& msg) {
    this->bus->sendRequest(msg);
}

void CacheController::receiveRequest(const Message& msg) {

}

void CacheController::sendReply(const Message& msg) {

}

void CacheController::receiveReply(const Message& msg) {
    bool flag = this->cache->sets[msg.address.setIndex].need_write_back(msg.address.tag);
    if (flag) {
        // changes take effect after dirty block is flushed back into memory
        this->cache->cpu.lock()->setHalt(TimeConfig::WriteBackMem);
    } else {
        this->cache->cpu.lock()->setHalt(0);
        this->cache->cpu.lock()->getInst()->execute(this->cache);
    }
}
