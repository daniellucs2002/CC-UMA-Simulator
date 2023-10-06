#include "traces/Trace.hpp"

void Instruction0::execute() const {
    std::cout << "Executing Instruction 0" << std::endl;
}

void Instruction1::execute() const {
    std::cout << "Executing Instruction 1" << std::endl;
}

void Instruction2::execute() const {
    std::cout << "Executing Instruction 2" << std::endl;
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
