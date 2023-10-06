#include <iostream>
#include <cassert>
#include <cstring>
#include "timer/Timer.hpp"
#include "cpu/CPU.hpp"
#include "config.hpp"

int main(int argc, char* argv[]) {
    assert(argc == 6);

    CacheConfig::cachesize = std::stoi(argv[3]);
    CacheConfig::associativity = std::stoi(argv[4]);
    CacheConfig::blocksize = std::stoi(argv[5]);

    std::string filename = std::string(argv[2]) + "_";  // e.g. _0.data

    if (strcmp(argv[1], "MESI") == 0) {
	
    } else if (strcmp(argv[1], "Dragon") == 0) {

    } else {
        std::cerr << "Invalid protocol selected" << std::endl;
        return 1;
    }
    
    std::shared_ptr<Timer> timer = std::make_shared<Timer>();

    const int num_cpus = 4;
    // Create CPUs using make_shared and attach to timer
    for (int i = 0; i < num_cpus; ++i) {
        auto cpu = std::make_shared<CPU>(i, filename);
        timer->attach(cpu);
    }

    while (true) {
        // start of a new clock cycle, notify all
        if (!timer->tick())
            break;
        // std::cout << "End of cycle " << timer->currentTime() << "..." << std::endl;
    }

    std::cout << "End of simulation at cycle " << timer->currentTime() << "..." << std::endl;

    return 0;
}
