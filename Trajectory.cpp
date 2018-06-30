//
// Created by andreas on 30.06.18.
//

#include <fstream>
#include "Trajectory.h"

Trajectory::Trajectory(const std::string &filepath) : filepath(filepath) {}

void Trajectory::addEntry(int timestamp, NBodySystem system) {
    std::vector<Point> entries;
    std::vector<Body *> systemContent = system.getSystemContent();

    for (auto &body: systemContent) {
        int id = body->id;
        double x = body->px;
        double y = body->py;
        Point point(id, x, y);
        entries.push_back(point);
    }
    this->entries.insert(std::pair<int, std::vector<Point>>(timestamp, entries));

}

std::ostream &operator<<(std::ostream &os, const Trajectory &trajectory) {
    for (int i = 0; i < trajectory.entries.size(); i++) {
        os << i << "{\n";
        std::vector<Point> currentRow = trajectory.entries.at(i);
        for (auto &p: currentRow) {
            os << p << "\n";
        }

    }
    return os;
}

void Trajectory::writeTrajectory() {
    std::ofstream myfile;
    myfile.open(this->filepath);
    myfile << "ts" << "," << "x" << "," << "y" << "\n";
    for (int ts = 0; ts < this->entries.size(); ts++) {
        for (auto &point: this->entries.at(ts)) {
            myfile << ts << "," << point.x << "," << point.y << "\n";

        }
    }
    myfile.close();
}
