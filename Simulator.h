//
// Created by andreas on 6/30/18.
//

#ifndef NBODY_CARO_SIMULATOR_H
#define NBODY_CARO_SIMULATOR_H


#include "NBodySystem.h"
#include "Trajectory.h"
#include <map>
#include <CL/cl2.hpp>

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
    double distanceBetween(Body *b1, Body *b2);

    std::vector<std::vector<Body * >> interactionMatrix;


    Trajectory trajectory = Trajectory("/home/andreas/Dropbox/Programmierung/Cpp/nbody-caro/output.data");

    void simulationStep(double dt);

    void buildInteractionMatrix();

    void calculateForces();

    void updateForces(double dt);
};

class ParallelSimulator : public Simulator {
public:
    ParallelSimulator(const NBodySystem &system);

    void run(int steps, double dt) override;

    void setupOpenCL();

    std::string loadKernelCode();

    void simulationStep(double d);

    void calculateInteractionBuffer();

    void calculateForces();

    void updateBodies();

    double *bodyToDouble3(Body *&pBody);


    std::vector<double>* masses;
    std::vector<double>* xs;
    std::vector<double>* ys;
};

#endif //NBODY_CARO_SIMULATOR_H
