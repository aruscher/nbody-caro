#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <CL/cl2.hpp>

using namespace std;
using namespace cv;

int main() {
    //CL SETUP
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0) {
        std::cout << " No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform = all_platforms[1];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_CPU, &all_devices);
    if (all_devices.size() == 0) {
        std::cout << " No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device = all_devices[0];
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";


    cl::Context context({default_device});
    cl::Program::Sources sources;


    //Load Image data
    string imagePath = "/home/andreas/Dropbox/Programmierung/Cpp/nbody-caro/testimg/fig.png";
    string imageChangePath = "/home/andreas/Dropbox/Programmierung/Cpp/nbody-caro/testimg/fig-change.png";

    Mat image;
    image = imread(imagePath, CV_LOAD_IMAGE_COLOR);

    int rows = image.size().height;
    int cols = image.size().width;
    int size = rows * cols;

    int R[rows][cols] = {0};
    int G[rows][cols] = {0};
    int B[rows][cols] = {0};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);
            B[i][j] = pixel.val[0];
            G[i][j] = pixel.val[1];
            R[i][j] = pixel.val[2];
        }
    }
    int flatB[size] = {0};
    int flatG[size] = {0};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * rows + j;
            flatB[index] = B[i][j];
            flatG[index] = G[i][j];
        }
    }


    //LOAD Kernel
    string kernelCode{R"CLC(
            __kernel void swapGB(__global int* gBuffer, __global int* bBuffer) {
                size_t id = (get_global_id(1) * get_global_size(0)) + get_global_id(0);
                printf("Kernel Id: %d\n",id);
                int gStore = gBuffer[id];
                int bStore = bBuffer[id];
                gBuffer[id] = bStore;
                bBuffer[id] = gStore;
                //printf("Swap g %d with b %d\n",gStore, bStore);
            }
       )CLC"};
    sources.push_back({kernelCode.c_str(), kernelCode.length()});

    cl::Program program(context, sources);
    if (program.build({default_device}) != CL_SUCCESS) {
        std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
        exit(1);
    }

    cl::CommandQueue queue(context, default_device);

    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                        rows * cols * sizeof(int), flatB);
    cl::Buffer buffer_G(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                        rows * cols * sizeof(int), flatG);

    queue.enqueueWriteBuffer(buffer_G, CL_TRUE, 0, rows * cols * sizeof(int), flatG);
    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, rows * cols * sizeof(int), flatB);
    cl::Kernel swapKernel(program, "swapGB");
    swapKernel.setArg(0, buffer_G);
    swapKernel.setArg(1, buffer_B);
    queue.enqueueNDRangeKernel(swapKernel, cl::NullRange, cl::NDRange(cols, rows));
    queue.enqueueReadBuffer(buffer_G, CL_TRUE, 0, rows * cols * sizeof(int), flatG);
    queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, rows * cols * sizeof(int), flatB);
    queue.finish();

    //Reconstruct matrices
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * rows + j;
            B[i][j] = flatB[index];
            G[i][j] = flatG[index];
        }
    }

    //Create-new image
    Mat changedImage = image.clone();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int rColor = R[i][j];
            int bColor = B[i][j];
            int gColor = G[i][j];
            changedImage.at<Vec3b>(i,j)[0] = bColor;
            changedImage.at<Vec3b>(i,j)[1] = gColor;
            changedImage.at<Vec3b>(i,j)[2] = rColor;
        }
    }
    imwrite(imageChangePath,changedImage);


    return 0;
}

//
//#include <iostream>
//#include <ctime>
//
//#include "Body.h"
//#include "NBodySystem.h"
//#include "Simulator.h"
//
//
//using namespace std;
//
//int main() {
//    cout << "Hello, World!" << endl;
//    NBodySystem sy;
//    double xRange[] = {-10000, 10000};
//    double yRange[] = {-10000, 10000};
//    double vRange[] = {0, 0};
//    double massRange[] = {1, 10};
////
//    int nBodies = 2;
//    for (int i = 0; i < nBodies; i++) {
//        sy.addRandomBody(i, xRange, yRange, vRange, massRange);
//    }
//
////    SequentialSimulator sim(sy);
//    ParallelSimulator sim(sy);
//
//    clock_t start = clock();
//    cout << "Start simulation" << endl;
//    sim.run(100000, 100000);
//    cout << "Complete Simulation" << endl;
//    int ms = (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000);
//    cout << "Took " << ms << " milliseconds" << endl;
//
//    return 0;
//}

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