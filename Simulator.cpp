//
// Created by andreas on 6/30/18.
//

#include <iostream>
#include <cmath>

#include "Simulator.h"

const NBodySystem &Simulator::getSystem() const {
    return system;
}

Simulator::Simulator(NBodySystem system) {
    this->system = system;

}

void SequentialSimulator::run(int steps, double dt) {
    int n = this->getSystem().getSystemContent().size();
    std::cout << "Run sequential simulation with n= " << n << " bodies" << std::endl;
    this->buildInteractionMatrix();
    this->trajectory.addEntry(0, this->getSystem());
    for (int i = 0; i < steps; i++) {
        std::cout << "Calculate " << i << "/" << steps << std::endl;
//        std::cout << "Begin Current system: " << this->getSystem() << std::endl;
        this->simulationStep(dt);
//        std::cout << "End Current system: " << this->getSystem() << std::endl;
//        std::cout << "-----------------------------" << std::endl;
        this->trajectory.addEntry(i + 1, this->getSystem());
    }
    this->trajectory.writeTrajectory();
}


void SequentialSimulator::simulationStep(double dt) {
    this->calculateForces();
    this->updateForces(dt);
}


double SequentialSimulator::distanceBetween(Body *b1, Body *b2) {
    double dx = b1->px - b2->px;
    double dy = b1->py - b2->py;
    return std::sqrt(dx * dx + dy * dy);
}

void SequentialSimulator::buildInteractionMatrix() {
    std::vector<Body *> systemContent = this->getSystem().getSystemContent();
    std::vector<std::vector<Body *>> bodyMatrix;
    for (auto &keyElement : systemContent) {
        std::vector<Body *> matrixRow;
        matrixRow.push_back(keyElement);
        for (auto &valueElement : systemContent) {
            if (keyElement != valueElement) {
                matrixRow.push_back(valueElement);
            }
        }
        bodyMatrix.push_back(matrixRow);
    }
    this->interactionMatrix = bodyMatrix;
}

void SequentialSimulator::updateForces(double dt) {
    std::vector<Body *> systemContent = this->getSystem().getSystemContent();
    for (auto &body: systemContent) {
        body->update(dt);
    }
}


void SequentialSimulator::calculateForces() {
    float softeningConstant = 0.1;
    float gravityConstant = 6.674e-11;

    std::vector<std::vector<Body *>> interactionMatrix = this->interactionMatrix;
    for (auto &matrixRow: interactionMatrix) {
        Body *currentBody = matrixRow.front();
        float totalFx = 0;
        float totalFy = 0;
        for (auto it = matrixRow.begin() + 1; it != matrixRow.end(); ++it) {
            double dX = (*it)->px - currentBody->px;
            double dY = (*it)->py - currentBody->py;
            double dist = this->distanceBetween(currentBody, (*it));

            totalFx += gravityConstant * (dX * (*it)->mass) /
                       (pow(dist * dist + softeningConstant * softeningConstant, 3 / 2));
            totalFy += gravityConstant * (dY * (*it)->mass) /
                       (pow(dist * dist + softeningConstant * softeningConstant, 3 / 2));
        }
        currentBody->fy = totalFy;
        currentBody->fx = totalFx;
    }
}


SequentialSimulator::SequentialSimulator(const NBodySystem &system) : Simulator(system) {}

ParallelSimulator::ParallelSimulator(const NBodySystem &system) : Simulator(system) {}

void ParallelSimulator::run(int steps, double dt) {
//    this->setupOpenCL();
    for (int i = 0; i < steps; i++) {
        this->simulationStep(dt);
    }
    //1.) calculate forces in parallel


    //2.) update bodies sequential
}

void ParallelSimulator::simulationStep(double d) {
    this->calculateInteractionBuffer();
    this->calculateForces();
    this->updateBodies();
}


void ParallelSimulator::calculateInteractionBuffer() {
    std::vector<Body *> systemContent = this->getSystem().getSystemContent();
    int n = systemContent.size();
    std::vector<std::vector<double *>> bodyMatrix;

    for (auto &keyElement : systemContent) {
        std::vector<double *> matrixRow;
        double *vectorBody = this->bodyToDouble3(keyElement);
        matrixRow.push_back(vectorBody);
        for (auto &valueElement : systemContent) {
            if (keyElement != valueElement) {
                double *vectorBody = this->bodyToDouble3(valueElement);
                matrixRow.push_back(vectorBody);
            }
        }
        bodyMatrix.push_back(matrixRow);
    }
    this->masses = new std::vector<double>();
    this->xs = new std::vector<double>();
    this->ys = new std::vector<double>();
    for (int i = 0; i < bodyMatrix.size(); i++) {
        for (int j = 0; j < bodyMatrix.at(i).size(); j++) {
            double *bodyVec = bodyMatrix.at(i).at(j);
            this->masses->push_back(bodyVec[0]);
            this->xs->push_back(bodyVec[1]);
            this->ys->push_back(bodyVec[2]);
        }
    }

}

std::string ParallelSimulator::loadKernelCode() {
    std::string kernelCode{R"CLC(
                double calculateDistance(double b1x, double b1y, double b2x, double b2y){
                    double dx = b1x-b2x;
                    double dy = b1y-b2y;
                    return sqrt(dx*dx+dy*dy);
                };

                double calculateForce(double dist, double b1p, double b2p, double b2m){
                    double soft = 0.1;
                    double gravityConstant = 6.674e-11;
                    double dP = b2p - b1p;
                    double up = dP*b2m;
                    //printf("dist %d",dist);
                    //double innerSqrt = dist*dist+soft*soft;
                    return 0;
                };

                kernel void calculateForces(global double *masses, global double *xs, global  double *ys, int  n) {
                    size_t id =  get_global_id(0);
                    printf("Kernel Id: %d\n",id);
                    double totalFx = 0;
                    double totalFy = 0;
                    int start = id*n;
                    int end  = start+n;
                    printf("Run from %d - %d\n",start,end);
                    double currentBodyX = xs[start];
                    double currentBodyY = xs[start];
                    double currentBodyMass = masses[start];
                    printf("Current body: %d %d %d\n", currentBodyX, currentBodyY, currentBodyMass);
//                    for(int i = start+1;i<end;i++){
//                        double otherBodyX = xs[i];
//                        double otherBodyY = ys[i];
//                        double otherBodyM = masses[i];
//                        double dist = calculateDistance(currentBodyX,currentBodyY, otherBodyX, otherBodyY);
//                        printf("Dist %d\n",dist);
//                        //totalFx += calculateForce(dist,currentBodyX,otherBodyX,otherBodyM);
//                        //totalFy += calculateForce(dist,currentBodyY,otherBodyY,otherBodyM);
//                    }
//                    printf("Total Force for Body %d: FX %d FY %d\n", id, totalFx,totalFy);
                };


      )CLC"};
    return kernelCode;
}

void ParallelSimulator::calculateForces() {
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);

    if (all_platforms.size() == 0) {
        std::cout << " No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform = all_platforms[0];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    // get default device (CPUs, GPUs) of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if (all_devices.size() == 0) {
        std::cout << " No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device = all_devices[0];
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";
    cl::Context context({default_device});
    cl::Program::Sources sources;
    std::string kernelCode = this->loadKernelCode();
    sources.push_back({kernelCode.c_str(), kernelCode.length()});

    cl::CommandQueue queue(context, default_device);


    cl::Program program(context, sources);
    if ((program).build({default_device}) != CL_SUCCESS) {
        std::cout << "Error building: " << (program).getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
        exit(1);
    }

    std::vector<Body *> systemContent = this->getSystem().getSystemContent();
    int n = systemContent.size();
    cl_int error = 0;

    double massToBuffer[n * n] = {0};
    double xsToBuffer[n * n] = {0};
    double ysToBuffer[n * n] = {0};

    for (int i = 0; i < n * n; i++) {
        xsToBuffer[i] = this->xs->at(i);
        ysToBuffer[i] = this->ys->at(i);
        massToBuffer[i] = this->masses->at(i);
    }

    size_t size = n * n * sizeof(double);

    cl::Buffer massBuffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY,
                          size);
    cl::Buffer xsBuffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY,
                        size);
    cl::Buffer ysBuffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY,
                        size);

    error = queue.enqueueWriteBuffer(massBuffer, CL_TRUE, 0, size, massToBuffer);
    error = queue.enqueueWriteBuffer(xsBuffer, CL_TRUE, 0, size, xsToBuffer);
    error = queue.enqueueWriteBuffer(ysBuffer, CL_TRUE, 0, size, ysToBuffer);

    cl::Kernel bodyKernel(program, "calculateForces", &error);
    error = bodyKernel.setArg(0, massBuffer);
    error = bodyKernel.setArg(1, xsBuffer);
    error = bodyKernel.setArg(2, ysBuffer);
    error = bodyKernel.setArg(3, n);


    error = queue.enqueueNDRangeKernel(bodyKernel, cl::NullRange, cl::NDRange(n));
    queue.finish();
}

void ParallelSimulator::updateBodies() {

}

double *ParallelSimulator::bodyToDouble3(Body *&pBody) {
    double mass = pBody->mass;
    double px = pBody->px;
    double py = pBody->py;
    return new double[3]{mass, px, py};;
}

