#include <iostream>
#include <memory>
#include <sstream>

class Trace {
public:
    virtual void execute() const = 0;
    virtual ~Trace() {}

    // Factory method to create instruction objects based on input string
    static std::unique_ptr<Trace> createInstruction(const std::string & line);
};

// load instruction
class Instruction0 : public Trace {
public:
    void execute() const override;
};

// store instruction
class Instruction1 : public Trace {
public:
    void execute() const override;
};

// other instructions
class Instruction2 : public Trace {
public:
    void execute() const override;
};
