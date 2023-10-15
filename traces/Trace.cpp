#include "traces/Trace.hpp"
#include "config.hpp"

unsigned int Instruction0::execute(std::shared_ptr<Cache> cache) const {
    return cache->read_addr(this->address);
}

unsigned int Instruction1::execute(std::shared_ptr<Cache> cache) const {
    return cache->write_addr(this->address);
}

unsigned int Instruction2::execute(std::shared_ptr<Cache> cache) const {
    return this->time;
}

unsigned int Instruction0::detect(std::shared_ptr<Cache> cache) const {
    return cache->detect_addr(this->address);
}

unsigned int Instruction1::detect(std::shared_ptr<Cache> cache) const {
    return cache->detect_addr(this->address);
}

unsigned int Instruction2::detect(std::shared_ptr<Cache> cache) const {
    return this->time;
}

std::shared_ptr<Trace> Trace::createInstruction(const std::string & line) {
    std::istringstream iss(line);
    int instructionType;
    unsigned int hexval;
    iss >> instructionType;
    iss >> std::hex >> hexval;

    switch (instructionType) {
        case 0: return std::make_shared<Instruction0>(hexval);
        case 1: return std::make_shared<Instruction1>(hexval);
        case 2: return std::make_shared<Instruction2>(hexval);
        default: throw std::runtime_error("Unknown instruction type");
    }
}
