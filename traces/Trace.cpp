#include "traces/Trace.hpp"

unsigned int Instruction0::execute(std::shared_ptr<Cache> cache) const {
    return cache->read_addr(this->address);
}

unsigned int Instruction1::execute(std::shared_ptr<Cache> cache) const {
    return cache->write_addr(this->address);
}

unsigned int Instruction2::execute(std::shared_ptr<Cache> cache) const {
    return this->time;
}

std::unique_ptr<Trace> Trace::createInstruction(const std::string & line) {
    std::istringstream iss(line);
    int instructionType;
    unsigned int hexval;
    iss >> instructionType;
    iss >> std::hex >> hexval;

    switch (instructionType) {
        case 0: return std::make_unique<Instruction0>(hexval);
        case 1: return std::make_unique<Instruction1>(hexval);
        case 2: return std::make_unique<Instruction2>(hexval);
        default: throw std::runtime_error("Unknown instruction type");
    }
}
