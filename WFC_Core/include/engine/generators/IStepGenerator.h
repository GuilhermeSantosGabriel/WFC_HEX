#ifndef STEP_GENERATOR_H
#define STEP_GENERATOR_H

class IStepGenerator {
public:
    virtual bool step() = 0;
};

#endif
