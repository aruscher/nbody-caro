//
// Created by andreas on 30.06.18.
//

#ifndef NBODY_CARO_POINT_H
#define NBODY_CARO_POINT_H


class Point {
public:
    Point(int id, double x, double y);

    int getId() const;

    double getX() const;

    double getY() const;

private:
    int id;
    double x;
    double y;
};


#endif //NBODY_CARO_POINT_H
