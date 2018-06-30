#include <iostream>
#include <ctime>
#include "Body.h"
#include "NBodySystem.h"
#include "Simulator.h"


using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    NBodySystem sy;
    Body b1(0, 0, 0, 0, 1000);
    Body b2(1, 0, 0, 0);
    sy.addBody(&b1);
    sy.addBody(&b2);

    SequentialSimulator sim(sy);

    clock_t start = clock();
    cout << "Start simulation" << endl;
    sim.run(400000, 1);
    cout << "Complete Simulation" << endl;
    int ms = (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000);
    cout << "Took " << ms << " milliseconds" << endl;


    return 0;
}