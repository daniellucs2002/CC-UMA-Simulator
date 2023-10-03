#include <iostream>
#include <cassert>
#include <cstring>
#include "timer/Timer.h"
#include "config.h"

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
    timer->tick();
    timer->tick();
    timer->tick();
    std::cout << timer->currentTime() << std::endl;
    return 0;
}

