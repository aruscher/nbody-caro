//
// Created by andreas on 30.06.18.
//

#include "Point.h"

Point::Point(int id, double x, double y) : id(id), x(x), y(y) {}


int Point::getId() const {
    return id;
}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}


std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "id: " << point.id << " x: " << point.x << " y: " << point.y;
    return os;
}
