//
// Created by andreas on 6/30/18.
//

#include <random>
#include <chrono>
#include "NBodySystem.h"

NBodySystem::NBodySystem() = default;

void NBodySystem::addBody(Body *b) {
    b->setID(this->counter);
    this->counter++;
    this->systemContent.push_back(b);
}

void NBodySystem::addBody(Body *b, int id) {
    (b)->setID(id);
    this->systemContent.push_back(b);

}


std::ostream &operator<<(std::ostream &os, const NBodySystem &system1) {
    os << "systemContent: \n";
    for (int i = 0; i < system1.systemContent.size(); i++) {
        os << *system1.systemContent.at(i) << "\n";
    }
    return os;
}

const std::vector<Body *> &NBodySystem::getSystemContent() const {
    return systemContent;
}

void NBodySystem::addRandomBody(int id, double *xrange, double *yrange, double *vrange, double *massrange) {
    unsigned seed = 10;
    std::default_random_engine generator(seed);

    std::uniform_real_distribution<double> xSampler(xrange[0], xrange[1]);
    std::uniform_real_distribution<double> ySampler(yrange[0], yrange[1]);
    std::uniform_real_distribution<double> vSampler(vrange[0], vrange[1]);
    std::uniform_real_distribution<double> massSampler(massrange[0], massrange[1]);

    Body generatedBody(xSampler(generator), ySampler(generator), vSampler(generator), vSampler(generator),
                       massSampler(generator));
    this->addBody(&generatedBody, id);

}



