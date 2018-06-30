//
// Created by andreas on 6/30/18.
//

#ifndef NBODY_CARO_SYSTEM_H
#define NBODY_CARO_SYSTEM_H


#include <vector>
#include <ostream>
#include "Body.h"

class System {

public:
    System();

    void addBody(Body b);

    std::vector<Body> getContent();

    friend std::ostream &operator<<(std::ostream &os, const System &system1);
    //void generateRandomBody();


private:
    std::vector<Body> systemContent;

};


#endif //NBODY_CARO_SYSTEM_H
