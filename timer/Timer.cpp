#include "timer/Timer.hpp"
#include "config.hpp"

void Timer::attach(std::shared_ptr<Observer> observer) {
    m_observers.push_back(observer);
}

bool Timer::tick() {
    m_ticks++;
    return notify(m_ticks);
}

bool Timer::notify(unsigned long now) {
    int cnt = 0;

    // in each time cycle, several things need to be done
    // step1: processor events, read instructions
    for (auto & observer : m_observers)
        if (!observer->update(now))
            cnt++;

    // step2: broadcast requests and process requests

    // step3: receive processing results and decide changes

    return (cnt != cpunums);
}

unsigned long Timer::currentTime() const {
    return m_ticks;
}
