//
// Created by andreas on 6/30/18.
//

#ifndef NBODY_CARO_SIMULATOR_H
#define NBODY_CARO_SIMULATOR_H


#include "NBodySystem.h"
#include <map>

class Simulator {
public:
    Simulator(NBodySystem system);

    virtual void run(int steps, double dt)= 0;

    const NBodySystem &getSystem() const;

private:
    NBodySystem system;
};


class SequentialSimulator : public Simulator {
public:
    SequentialSimulator(const NBodySystem &system);

    void run(int steps, double dt) override;

private:
    double distanceBetween(Body b1, Body b2);

    std::vector<std::vector<Body>> interactionMatrix;

    void simulationStep(double dt);

    void buildInteractionMatrix();

    void calculateForces();

    void updateForces(double dt);
};

class ParallelSimulator : public Simulator {
public:
    ParallelSimulator(const NBodySystem &system);

};

#endif //NBODY_CARO_SIMULATOR_H
