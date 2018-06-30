#include <iostream>
#include "Body.h"
#include "System.h"
#include "Simulator.h"


using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    System sy;
    Body b1(0, 0, 0, 0);
    Body b2(0, 10, 0, 0);
    sy.addBody(b1);
    sy.addBody(b2);

    SequentialSimulator sim(sy);




    cout << sy << endl;


    return 0;
}