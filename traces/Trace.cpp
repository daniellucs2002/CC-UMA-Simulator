#include "traces/Trace.hpp"

void Instruction0::execute(std::shared_ptr<Cache> cache) const {
    unsigned int cycles = cache->read_addr(this->address);
    std::cout << "Load: " << cycles << std::endl;
}

void Instruction1::execute(std::shared_ptr<Cache> cache) const {
    unsigned int cycles = cache->write_addr(this->address);
    std::cout << "Store: " << cycles << std::endl;
}

void Instruction2::execute(std::shared_ptr<Cache> cache) const {
    unsigned int cycles = this->time;
    std::cout << "Exec: " << cycles << std::endl;
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
