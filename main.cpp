

#include <iostream>
#include <ctime>

#include "Body.h"
#include "NBodySystem.h"
#include "Simulator.h"


using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    NBodySystem sy;
    double xRange[] = {-10000, 10000};
    double yRange[] = {-10000, 10000};
    double vRange[] = {0, 0};
    double massRange[] = {1, 10};
//
    int nBodies = 2;
    for (int i = 0; i < nBodies; i++) {
        sy.addRandomBody(i, xRange, yRange, vRange, massRange);
    }

//    SequentialSimulator sim(sy);
    ParallelSimulator sim(sy);

    clock_t start = clock();
    cout << "Start simulation" << endl;
    sim.run(100000, 100000);
    cout << "Complete Simulation" << endl;
    int ms = (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000);
    cout << "Took " << ms << " milliseconds" << endl;

    return 0;
}

////#include <cmath>
////
////
////
////class Body {
////public:
////    Body(int id, double x, double y, int mass, double vx, double vy, double fx, double fy) : id(id), x(x), y(y), mass(mass),
////                                                                                       vx(vx), vy(vy), fx(fx), fy(fy) {}
////
////    int id;
////    double x;
////    double y;
////    int mass;
////    double vx;
////    double vy;
////    double fx;
////    double fy;
////};
////
////void doForce(Body *b1, Body *b2) {
////    // 1 geschwindigkeit gegeben =  kräfte
////    /*mb - my body / ob - other body
////    diffX = x1(mb) - x2(ob)
////    diffY = y1(mb) - y2(ob)
////    soft = softening faktor (bsp. 0.01)
////    betrag = sqrt(diffX * diifX + diffY * diffY);
////    fX =  gravitationskonstante * Sum( (obMasse * diffX)/( betrag² + soft²)^(3/2) )
////    fY =  gravitationskonstante * Sum( (obMasse * diffY)/( betrag² + soft²)^(3/2) )
////    */
////    // body 1
////    double soft = 0.01;
////    double gravity = 6.674e-11;
////    //TODO ERROR
////    double diffx = b2->x - b1->x;
////    double diffy = b2->y - b1->y;
////    double betrag = sqrt(diffx * diffx + diffy * diffy);
////
////    b1->fx = gravity * ((b2->mass * diffx) / pow(((betrag * betrag) + (soft * soft)), 3 / 2));
////    b1->fy = gravity * ((b2->mass * diffy) / pow(((betrag * betrag) + (soft * soft)), 3 / 2));
////}
////
////void doVelocity(Body *b1, int time) {
////    // 2 Beschleunigungsvektor + 1
////    /*
////    Zeit = 1;
////    velocityX = (Zeit * fX) / masse(mb)
////    velocityY = (Zeit * fY) / masse(mb)
////    */
////    b1->vx = (time * b1->fx) / b1->mass;
////    b1->vy = (time * b1->fy) / b1->mass;
////}
////
////void doPosition(Body *b1) {
////    // 3 Position + 2
////    /*
////    posX += velocityX
////    posY += velocityY
////    */
////    b1->x += b1->vx;
////    b1->y += b1->vy;
////}
////
////int main() {
////
////    int time = 100000;
////    int steps = 100000;
////    Body *b1 = new Body(1, 0.0, 0.0, 1, 0.0, 0.0, 0.0, 0.0);
////    Body *b2 = new Body(2, 0.0, 1.0, 1, 0.0, 0.0, 0.0, 0.0);
////    cout << " Start: x1: " << b1->x << " y1: " << b1->y << " x2: " << b2->x << " y2: " << b2->y << " \n";
////
////    //
////    for (int i = 0; i < steps; i++) {
////
////        // force
////        doForce(b1, b2);
////        doForce(b2, b1);
////
////        //do velocity
////        doVelocity(b1, time);
////        doVelocity(b2, time);
////
////        //do pos
////        doPosition(b1);
////        doPosition(b2);
////
////        cout << "timestep: " << i << " x1: " << b1->x << " y1: " << b1->y << " x2: " << b2->x << " y2: " << b2->y
////             << " \n";
////    }
////    cin.get();
////}