#include "CacheController.hpp"

CacheController::CacheController(int i, std::shared_ptr<Bus> b, std::shared_ptr<Cache> c)
    : id(i), bus(b), cache(c)
{
    
}

int CacheController::getId() const {
    return id;
}

void CacheController::sendRequest(const Message& msg) {

}

void CacheController::receiveRequest(const Message& msg) {

}

void CacheController::sendReply(const Message& msg) {

}

void CacheController::receiveReply(const Message& msg) {

}
