#include "cpu/CPU.hpp"

CPU::CPU(int id) {
    this->cpu_id = id;
    cout << "Constructing CPU " << this->cpu_id << "..." << endl;
}

void CPU::update() {

}
