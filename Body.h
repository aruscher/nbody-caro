//
// Created by andreas on 6/30/18.
//

#ifndef NBODY_CARO_BODY_H
#define NBODY_CARO_BODY_H


#include <ostream>

class Body {

public:
    //Position
    float px, py = 0.0f;
    //Velocity
    float vx, vy = 0.0f;
    //Force
    float fx, fy = 0.0f;
    //Mass
    float mass = 0.0f;


    Body(float px, float py, float vx, float vy, float mass);

    Body(float px, float py, float vx, float vy);

    friend std::ostream &operator<<(std::ostream &os, const Body &body);

    float distanceTo(Body body);

};


#endif //NBODY_CARO_BODY_H
