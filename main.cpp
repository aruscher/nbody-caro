#include <iostream>
#include <ctime>
#include "Body.h"
#include "NBodySystem.h"
#include "Simulator.h"


using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    NBodySystem sy;
    double xRange[] = {-10, 10};
    double yRange[] = {-10, 10};
    double vRange[] = {0, 1};
    double massRange[] = {10, 100};
//
//    int nBodies = 2;
//    for (int i = 0; i < nBodies; i++) {
//        sy.addRandomBody(i, xRange, yRange, vRange, massRange);
//    }
    Body b1(0, 0, 0, 0, 1);
    Body b2(1, 0, 0, 0, 1);
    Body b3(1, 1, 0, 0, 1);
    Body b4(1, 2, 0, 0, 1);
    Body b5(5, 1, 0, 0, 1);
    Body b6(10, 1, 0, 0, 1);
    sy.addBody(&b1);
    sy.addBody(&b2);
    sy.addBody(&b3);
    sy.addBody(&b4);
    sy.addBody(&b5);
    sy.addBody(&b6);

    SequentialSimulator sim(sy);

    clock_t start = clock();
    cout << "Start simulation" << endl;
    sim.run(10000, 1000);
    cout << "Complete Simulation" << endl;
    int ms = (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000);
    cout << "Took " << ms << " milliseconds" << endl;

    return 0;
}