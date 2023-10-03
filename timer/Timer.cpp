#include "timer/Timer.h"

void Timer::attach(std::shared_ptr<Observer> observer) {
    m_observers.push_back(observer);
}

void Timer::tick() {
    m_ticks++;
    notify();
}

void Timer::notify() {
    for (auto & observer : m_observers)
	observer->update();
}

unsigned long Timer::currentTime() const {
    return m_ticks;
}

