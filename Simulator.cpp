//
// Created by andreas on 6/30/18.
//

#include <iostream>
#include <cmath>
#include "Simulator.h"

const NBodySystem &Simulator::getSystem() const {
    return system;
}

Simulator::Simulator(NBodySystem system) {
    this->system = system;

}

void SequentialSimulator::run(int steps, double dt) {
    int n = this->getSystem().getSystemContent().size();
    std::cout << "Run sequential simulation with n= " << n << " bodies" << std::endl;
    this->buildInteractionMatrix();
    this->trajectory.addEntry(0, this->getSystem());
    for (int i = 0; i < steps; i++) {
        std::cout << "Begin Current system: " << this->getSystem() << std::endl;
        this->simulationStep(dt);
        std::cout << "End Current system: " << this->getSystem() << std::endl;
        std::cout << "-----------------------------" << std::endl;
        this->trajectory.addEntry(i + 1, this->getSystem());
    }
    this->trajectory.writeTrajectory();
}


void SequentialSimulator::simulationStep(double dt) {
    this->calculateForces();
    this->updateForces(dt);
}


double SequentialSimulator::distanceBetween(Body *b1, Body *b2) {
    double dx = b1->px - b2->px;
    double dy = b1->py - b2->py;
    return std::sqrt(dx * dx + dy * dy);
}

void SequentialSimulator::buildInteractionMatrix() {
    std::vector<Body *> systemContent = this->getSystem().getSystemContent();
    std::vector<std::vector<Body *>> bodyMatrix;
    for (auto &keyElement : systemContent) {
        std::vector<Body *> matrixRow;
        matrixRow.push_back(keyElement);
        for (auto &valueElement : systemContent) {
            if (keyElement != valueElement) {
                matrixRow.push_back(valueElement);
            }
        }
        bodyMatrix.push_back(matrixRow);
    }
    this->interactionMatrix = bodyMatrix;
}

void SequentialSimulator::updateForces(double dt) {
    std::vector<Body *> systemContent = this->getSystem().getSystemContent();
    for (auto &body: systemContent) {
        body->update(dt);
    }
}


void SequentialSimulator::calculateForces() {
    float softeningConstant = 0.1;
    float gravityConstant = 6.674e-11;

    std::vector<std::vector<Body *>> interactionMatrix = this->interactionMatrix;
    for (auto &matrixRow: interactionMatrix) {
        Body *currentBody = matrixRow.front();
        float totalFx = 0;
        float totalFy = 0;
        for (auto it = matrixRow.begin() + 1; it != matrixRow.end(); ++it) {
            double dX = (*it)->px - currentBody->px;
            double dY = (*it)->py - currentBody->py;
            double dist = this->distanceBetween(currentBody, (*it));

            totalFx += gravityConstant * (dX * (*it)->mass) /
                       (pow(dist * dist + softeningConstant * softeningConstant, 3 / 2));
            totalFy += gravityConstant * (dY * (*it)->mass) /
                       (pow(dist * dist + softeningConstant * softeningConstant, 3 / 2));
        }
        currentBody->fy = totalFy;
        currentBody->fx = totalFx;
    }
}

ParallelSimulator::ParallelSimulator(const NBodySystem &system) : Simulator(system) {}

SequentialSimulator::SequentialSimulator(const NBodySystem &system) : Simulator(system) {}
