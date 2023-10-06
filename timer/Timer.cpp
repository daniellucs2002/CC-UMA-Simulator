#include "timer/Timer.h"

void Timer::attach(std::shared_ptr<Observer> observer) {
    m_observers.push_back(observer);
}

bool Timer::tick() {
    m_ticks++;
    return notify(m_ticks);
}

bool Timer::notify(unsigned long now) {
    int cnt = 0;
    for (auto & observer : m_observers)
        if (!observer->update(now))
            cnt++;
    return (cnt != 4);
}

unsigned long Timer::currentTime() const {
    return m_ticks;
}
