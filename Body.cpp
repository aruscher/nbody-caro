//
// Created by andreas on 6/30/18.
//

#include <iostream>
#include <cmath>
#include "Body.h"

Body::Body(float px, float py, float vx, float vy, float mass) : px(px), py(py), vx(vx), vy(vy), mass(mass) {}

Body::Body(float px, float py, float vx, float vy) : px(px), py(py), vx(vx), vy(vy) {
    mass = 1;
}


std::ostream &operator<<(std::ostream &os, const Body &body) {
    os << "px: " << body.px << " py: " << body.py << " vx: " << body.vx << " vy: " << body.vy << " fx: " << body.fx
       << " fy: " << body.fy << " mass: " << body.mass;
    return os;
}

float Body::distanceTo(Body body) {
    float dx = this->px-body.px;
    float dy = this->px-body.px;
    return std::sqrt(dx*dx+dy*dy);
}
