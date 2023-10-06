#include "timer/Timer.h"
#include <iostream>

using namespace std;

class CPU : public Observer {
private:
    int cpu_id;

public:
    CPU(int id);
    void update() override;
};
