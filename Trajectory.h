//
// Created by andreas on 30.06.18.
//

#ifndef NBODY_CARO_TRAJECTORY_H
#define NBODY_CARO_TRAJECTORY_H


#include <string>
#include <map>
#include "NBodySystem.h"
#include "Point.h"

class Trajectory {
public:
    Trajectory(const std::string &filepath);

    void addEntry(int timestamp, NBodySystem system);

private:
    std::string filepath;
    std::map<int, std::vector<Point>> entries;
};


#endif //NBODY_CARO_TRAJECTORY_H
