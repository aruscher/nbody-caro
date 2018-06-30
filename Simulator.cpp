//
// Created by andreas on 6/30/18.
//

#include "Simulator.h"

Simulator::Simulator(System system) {
    this->system = system;

}

SequentialSimulator::SequentialSimulator(const System &system) : Simulator(system) {}

void SequentialSimulator::run(int steps, float dt) {

}


ParallelSimulator::ParallelSimulator(const System &system) : Simulator(system) {}
