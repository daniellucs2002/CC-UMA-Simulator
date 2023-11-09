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
    cpu_stats.emplace_back(std::make_shared<Statistics>());  // for the bus, index is cpunums

    while (true) {
        // start of a new clock cycle, notify all
        if (!timer->tick())
            break;
        // std::cout << "End of cycle " << timer->currentTime() << "..." << std::endl;
    }

    std::cout << std::endl;
    std::cout << "End of simulation at cycle " << timer->currentTime() << "..." << std::endl;

    // print all the statistics
    for (int i = 0; i < cpunums; ++i) {  // print cpu[i]
        std::cout << "=== Statistics about CPU " << i << " ===" << std::endl;
        // sum of execution time
        std::cout << "Finished at cycle " << cpu_stats[i]->GetCount("sum_execution_time") << std::endl;
        // number of compute cycles per core
        std::cout << "Compute Cycles Number is " << cpu_stats[i]->GetCount("compute_cycles") << std::endl;
        // number of load/store instructions per core
        std::cout << "Load Number: " << cpu_stats[i]->GetCount("load_number") << " Store Number: "
            << cpu_stats[i]->GetCount("store_number") << std::endl;
        // number of idle cycles per core
        std::cout << "Idle Cycles Number is " << cpu_stats[i]->GetCount("sum_execution_time") -
            cpu_stats[i]->GetCount("compute_cycles") << std::endl;
        // data cache miss rate
        std::cout << "Data Cache Miss Rate: " << 
            (100 - (cpu_stats[i]->GetCount("cache_hit") * 100) / (cpu_stats[i]->GetCount("load_number") + 
            cpu_stats[i]->GetCount("store_number"))) << "%" << std::endl;
    }

    std::cout << "=== Statistics about the Bus ===" << std::endl;
    // Amount of Data traffic in bytes on the bus
    std::cout << "Data Traffic: " << cpu_stats[cpunums]->GetCount("data_traffic") << " bytes" << std::endl;
    // Number of invalidations or updates on the bus
    if (cpu_stats[cpunums]->GetCount("invalidation") != 0) {
        std::cout << "Number of Invalidations: " << cpu_stats[cpunums]->GetCount("invalidation") << std::endl;
    } else {
        std::cout << "Number of Updates: " << cpu_stats[cpunums]->GetCount("update") << std::endl;
    }
    // Distribution of accesses to private data versus shared data
    std::cout << "private access : public access = " << 
        (float)cpu_stats[cpunums]->GetCount("private") / cpu_stats[cpunums]->GetCount("public") << std::endl;

    return 0;
}
