#pragma once

#include <vector>
#include <memory>
#include "CacheController.hpp"
#include "CacheSet.hpp"
#include "config.hpp"

class CacheController;

// This is a blocking cache!
class Cache {
private:
    int cachesize;
    int associativity;
    int blocksize;
    int set;  // set number

    std::vector<CacheSet> sets;

    std::weak_ptr<CacheController> controller;

    CacheAddress parseAddress(unsigned int address);

public:
    Cache(int cachesize, int associativity, int blocksize);

    // input: address to be visited
    // output: cycles needed for completion
    unsigned int read_addr(unsigned int address);

    // input: address to be written to
    // ouput: cycles it takes to write address
    unsigned int write_addr(unsigned int address);

    void setController(const std::shared_ptr<CacheController>& ctrl) {
        this->controller = ctrl;
    }
};
