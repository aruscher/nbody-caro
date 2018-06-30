//
// Created by andreas on 6/30/18.
//

#ifndef NBODY_CARO_SYSTEM_H
#define NBODY_CARO_SYSTEM_H


#include <vector>
#include <ostream>
#include "Body.h"

class NBodySystem {

public:
    NBodySystem();

    void addBody(Body *b);

    friend std::ostream &operator<<(std::ostream &os, const NBodySystem &system1);
    //void generateRandomBody();

    const std::vector<Body*> &getSystemContent() const;


private:
    std::vector<Body *> systemContent;
    int counter = 0;
};


#endif //NBODY_CARO_SYSTEM_H
