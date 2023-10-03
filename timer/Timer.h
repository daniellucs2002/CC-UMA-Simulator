#include <memory>
#include <list>

class Observer {
public:
    virtual void update() = 0;
};

class Timer {
    unsigned long m_ticks;
    std::list<std::shared_ptr<Observer>> m_observers;

public:
    Timer() : m_ticks(0) {}
    void attach(std::shared_ptr<Observer> observer);
    void tick();
    void notify();
    unsigned long currentTime() const;
};

