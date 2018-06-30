#include <iostream>
#include <ctime>
#include "Body.h"
#include "NBodySystem.h"
#include "Simulator.h"


using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    NBodySystem sy;
    double xRange[] = {-1000, 1000};
    double yRange[] = {-1000, 1000};
    double vRange[] = {0, 0};
    double massRange[] = {1,10};
//
    int nBodies = 5;
    for (int i = 0; i < nBodies; i++) {
        sy.addRandomBody(i, xRange, yRange, vRange, massRange);
    }
//    Body b1(0, 0, 0, 0, 1);
//    Body b2(1, 0, 0, 0, 1);
//    Body b3(1, 1, 0, 0, 1);
//    Body b4(1, 2, 0, 0, 1);
//    Body b5(5, 1, 0, 0, 1);
//    Body b6(10, 1, 0, 0, 1);
//    sy.addBody(&b1);
//    sy.addBody(&b2);
//    sy.addBody(&b3);
//    sy.addBody(&b4);
//    sy.addBody(&b5);
//    sy.addBody(&b6);

    SequentialSimulator sim(sy);

    clock_t start = clock();
    cout << "Start simulation" << endl;
    sim.run(1000000, 0.09);
    cout << "Complete Simulation" << endl;
    int ms = (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000);
    cout << "Took " << ms << " milliseconds" << endl;

    return 0;
}