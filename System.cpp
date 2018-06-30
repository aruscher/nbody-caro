//
// Created by andreas on 6/30/18.
//

#include "System.h"

System::System() = default;

void System::addBody(Body b) {
    this->systemContent.push_back(b);
}


std::ostream &operator<<(std::ostream &os, const System &system1) {
    os << "systemContent: \n";
    for (int i = 0; i < system1.systemContent.size(); i++) {
        os << system1.systemContent.at(i) << "\n";
    }
    return os;
}

std::vector<Body> System::getContent() {
    return this->systemContent;
}
