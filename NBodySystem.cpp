//
// Created by andreas on 6/30/18.
//

#include "NBodySystem.h"

NBodySystem::NBodySystem() = default;

void NBodySystem::addBody(Body b) {
    b.setID(this->counter);
    this->counter++;
    this->systemContent.push_back(b);
}


std::ostream &operator<<(std::ostream &os, const NBodySystem &system1) {
    os << "systemContent: \n";
    for (int i = 0; i < system1.systemContent.size(); i++) {
        os << system1.systemContent.at(i) << "\n";
    }
    return os;
}

const std::vector<Body> &NBodySystem::getSystemContent() const {
    return systemContent;
}


