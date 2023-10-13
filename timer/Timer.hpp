#pragma once

#include <memory>
#include <list>
#include <limits>
#include "bus/Bus.hpp"
#include "cpu/CPU.hpp"

class Observer;
class Bus;

class Timer {
    unsigned long m_ticks;
    std::list<std::shared_ptr<Observer>> m_observers;
    std::shared_ptr<Bus> bus;

public:
    Timer() : m_ticks(std::numeric_limits<unsigned long>::max()) {}
    void attach(std::shared_ptr<Observer> observer);
    bool tick();
    bool notify(unsigned long now);
    unsigned long currentTime() const;

    void setBus(std::shared_ptr<Bus> bus) {this->bus = bus;}
    std::shared_ptr<Bus> getBus() {return this->bus;}
};
