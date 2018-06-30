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
    double massRange[] = {1,10};
//
    int nBodies = 200;
    for (int i = 0; i < nBodies; i++) {
        sy.addRandomBody(i, xRange, yRange, vRange, massRange);
    }

    SequentialSimulator sim(sy);

    clock_t start = clock();
    cout << "Start simulation" << endl;
    sim.run(100000, 100000);
    cout << "Complete Simulation" << endl;
    int ms = (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000);
    cout << "Took " << ms << " milliseconds" << endl;

    return 0;
}