//
// Created by andreas on 30.06.18.
//

#ifndef NBODY_CARO_POINT_H
#define NBODY_CARO_POINT_H


#include <ostream>

class Point {
public:
    Point(int id, double x, double y);

    int getId() const;

    double getX() const;

    double getY() const;

    friend std::ostream &operator<<(std::ostream &os, const Point &point);

    int id;
    double x;
    double y;

};


#endif //NBODY_CARO_POINT_H
