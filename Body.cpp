//
// Created by andreas on 6/30/18.
//

#include <iostream>
#include <cmath>
#include "Body.h"

Body::Body(double px, double py, double vx, double vy, double mass) : px(px), py(py), vx(vx), vy(vy), mass(mass) {}

Body::Body(double px, double py, double vx, double vy) : px(px), py(py), vx(vx), vy(vy) {
    mass = 1;
}

void Body::setID(int i) {
    this->id = i;

}


bool Body::operator==(const Body &rhs) const {
    return px == rhs.px &&
           py == rhs.py &&
           vx == rhs.vx &&
           vy == rhs.vy &&
           fx == rhs.fx &&
           fy == rhs.fy &&
           mass == rhs.mass &&
           id == rhs.id;
}

bool Body::operator!=(const Body &rhs) const {
    return !(rhs == *this);
}

bool Body::operator<(const Body &rhs) const {
    if (px < rhs.px)
        return true;
    if (rhs.px < px)
        return false;
    if (py < rhs.py)
        return true;
    if (rhs.py < py)
        return false;
    if (vx < rhs.vx)
        return true;
    if (rhs.vx < vx)
        return false;
    if (vy < rhs.vy)
        return true;
    if (rhs.vy < vy)
        return false;
    if (fx < rhs.fx)
        return true;
    if (rhs.fx < fx)
        return false;
    if (fy < rhs.fy)
        return true;
    if (rhs.fy < fy)
        return false;
    if (mass < rhs.mass)
        return true;
    if (rhs.mass < mass)
        return false;
    return id < rhs.id;
}

bool Body::operator>(const Body &rhs) const {
    return rhs < *this;
}

bool Body::operator<=(const Body &rhs) const {
    return !(rhs < *this);
}

bool Body::operator>=(const Body &rhs) const {
    return !(*this < rhs);
}

void Body::update(double dt) {
    this->vx += dt * (this->fx / this->mass);
    this->vy += dt * (this->fy / this->mass);
    double norm = sqrt(this->vy*this->vy+this->vx*this->vx);

    this->px += dt * (this->vx/norm);
    this->py += dt * (this->vy/norm);
    this->fx = 0;
    this->fy = 0;
}

std::ostream &operator<<(std::ostream &os, const Body &body) {
    os << "px: " << body.px << " py: " << body.py << " vx: " << body.vx << " vy: " << body.vy << " fx: " << body.fx
       << " fy: " << body.fy << " mass: " << body.mass << " id: " << body.id;
    return os;
}
