#include "traces/Trace.hpp"

void Instruction0::execute(std::shared_ptr<Cache> cache) const {

}

void Instruction1::execute(std::shared_ptr<Cache> cache) const {

}

void Instruction2::execute(std::shared_ptr<Cache> cache) const {

}

std::unique_ptr<Trace> Trace::createInstruction(const std::string & line) {
    std::istringstream iss(line);
    int instructionType;
    iss >> instructionType;

    switch (instructionType) {
        case 0: return std::make_unique<Instruction0>();
        case 1: return std::make_unique<Instruction1>();
        case 2: return std::make_unique<Instruction2>();
        default: throw std::runtime_error("Unknown instruction type");
    }
}
