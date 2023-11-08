#include <iostream>
#include <cassert>
#include <cstring>
#include "timer/Timer.hpp"
#include "cpu/CPU.hpp"
#include "bus/Bus.hpp"
#include "cache/CacheController.hpp"
#include "config.hpp"

#include "protocols/MesiProtocol.hpp"
#include "protocols/MoesiProtocol.hpp"
#include "protocols/DragonProtocol.hpp"

int main(int argc, char* argv[]) {
    assert(argc == 7);

    CacheConfig::cachesize = std::stoi(argv[3]);
    CacheConfig::associativity = std::stoi(argv[4]);
    CacheConfig::blocksize = std::stoi(argv[5]);

    std::string filename = std::string(argv[2]) + "_";  // e.g. _0.data

    if (strcmp(argv[1], "MESI") == 0) {
        protocol = std::make_shared<MESICacheCoherenceProtocol>();
    } else if (strcmp(argv[1], "MOESI") == 0) {
        protocol = std::make_shared<MOESICacheCoherenceProtocol>();
    } else if (strcmp(argv[1], "Dragon") == 0) {
        protocol = std::make_shared<DragonCacheCoherenceProtocol>();
    } else {
        std::cerr << "Invalid protocol selected" << std::endl;
        return 1;
    }
    
    std::shared_ptr<Timer> timer = std::make_shared<Timer>();
    std::shared_ptr<Bus> bus = std::make_shared<Bus>();
    timer->setBus(bus);  // invoke bus transactions

    cpunums = std::stoi(argv[6]);  // passed as a command line argument
    // Create CPUs using make_shared and attach to timer
    for (int i = 0; i < cpunums; ++i) {
        // cache & cache controller, cache & cpu need to point to each other
        auto cpu = std::make_shared<CPU>(i, filename);
        cpu->read_cache()->setCPU(cpu);
        timer->attach(cpu);
        // create corresponding cache controllers
        auto controller = std::make_shared<CacheController>(i, bus, cpu->read_cache());
        cpu->read_cache()->setController(controller);
        bus->registerCache(controller);
        // one Statistics class for each cpu core
        cpu_stats.emplace_back(std::make_shared<Statistics>());
    }

    while (true) {
        // start of a new clock cycle, notify all
        if (!timer->tick())
            break;
        // std::cout << "End of cycle " << timer->currentTime() << "..." << std::endl;
    }

    std::cout << "End of simulation at cycle " << timer->currentTime() << "..." << std::endl;

    // print all the statistics
    for (int i = 0; i < cpunums; ++i) {  // print cpu[i]
        std::cout << "=== Statistics about CPU " << i << " ===" << std::endl;
        // sum of execution time
        std::cout << "Finished at cycle " << cpu_stats[i]->GetCount("sum_execution_time") << endl;
    }

    return 0;
}
