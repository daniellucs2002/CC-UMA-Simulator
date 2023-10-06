#pragma once

#include <memory>
#include <list>
#include <limits>

class Observer {
public:
    virtual bool update(unsigned long now) = 0;
    virtual ~Observer() = default;
};

class Timer {
    unsigned long m_ticks;
    std::list<std::shared_ptr<Observer>> m_observers;

public:
    Timer() : m_ticks(std::numeric_limits<unsigned long>::max()) {}
    void attach(std::shared_ptr<Observer> observer);
    bool tick();
    bool notify(unsigned long now);
    unsigned long currentTime() const;
};
