#include <iostream>
#include "Body.h"
#include "NBodySystem.h"
#include "Simulator.h"


using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    NBodySystem sy;
    Body b1(0, 0, 0, 0);
    Body b2(0, 1, 0, 0);
    sy.addBody(&b1);
    sy.addBody(&b2);

    SequentialSimulator sim(sy);

    sim.run(10000, 1);


    return 0;
}