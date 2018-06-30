//
// Created by andreas on 6/30/18.
//

#ifndef NBODY_CARO_SIMULATOR_H
#define NBODY_CARO_SIMULATOR_H


#include "System.h"

class Simulator {
public:
    Simulator(System system);

    virtual void run(int steps, float dt)= 0;


private:
    System system;
};


class SequentialSimulator : public Simulator {
public:
    SequentialSimulator(const System &system);

    void run(int steps, float dt) override;

};

class ParallelSimulator : public Simulator {
public:
    ParallelSimulator(const System &system);

};

#endif //NBODY_CARO_SIMULATOR_H
