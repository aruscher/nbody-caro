//
// Created by andreas on 30.06.18.
//

#include "Trajectory.h"

Trajectory::Trajectory(const std::string &filepath) : filepath(filepath) {}

void Trajectory::addEntry(int timestamp, NBodySystem system) {




    for (auto &body: system.getSystemContent()) {

    }
}