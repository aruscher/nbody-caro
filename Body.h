//
// Created by andreas on 6/30/18.
//

#ifndef NBODY_CARO_BODY_H
#define NBODY_CARO_BODY_H


#include <ostream>

class Body {

public:
    //Position
    double px, py = 0;
    //Velocity
    double vx, vy = 0;
    //Force
    double fx, fy = 0;
    //Mass
    double mass = 0;

    int id;

    Body(double px, double py, double vx, double vy, double mass);

    Body(double px, double py, double vx, double vy);

    void setID(int id);

    void update(double dt);


    bool operator==(const Body &rhs) const;

    bool operator!=(const Body &rhs) const;

    bool operator<(const Body &rhs) const;

    bool operator>(const Body &rhs) const;

    bool operator<=(const Body &rhs) const;

    bool operator>=(const Body &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Body &body);


};


#endif //NBODY_CARO_BODY_H
