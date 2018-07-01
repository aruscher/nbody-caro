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


std::string ParallelSimulator::loadKernelCode() {
    std::string kernelCode{R"CLC(
                __kernel void calculateForces(__global const double* masses, __global const double* xs, __global const double* ys) {
                    size_t id = (get_global_id(1) * get_global_size(0)) + get_global_id(0);
                    printf("Kernel Id: %d\n",id);
               };

                __kernel void debug(){
                    size_t id = (get_global_id(1) * get_global_size(0)) + get_global_id(0);
                    printf("Kernel Id: %d\n",id);
                };

      )CLC"};
    return kernelCode;
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
    if (program.build({default_device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
        exit(1);
    }


    std::vector<Body *> systemContent = this->getSystem().getSystemContent();
    int n = systemContent.size();
    double *masses = this->masses->data();
    double *xs = this->xs->data();
    double *ys = this->ys->data();

    cl::Buffer massBuffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                          n * n * sizeof(double));
    cl::Buffer xsBuffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                        n * n * sizeof(double));
    cl::Buffer ysBuffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                        n * n * sizeof(double));
    cl::Kernel bodyKernel(program, "calculateForces");
    bodyKernel.setArg(0, massBuffer);
    bodyKernel.setArg(1, xsBuffer);
    bodyKernel.setArg(2, ysBuffer);
//
    queue.enqueueWriteBuffer(massBuffer, CL_TRUE, 0, n * n * sizeof(double), masses);
    queue.enqueueWriteBuffer(xsBuffer, CL_TRUE, 0, n * n * sizeof(double), xs);
    queue.enqueueWriteBuffer(ysBuffer, CL_TRUE, 0, n * n * sizeof(double), ys);
    queue.enqueueNDRangeKernel(bodyKernel, cl::NullRange, cl::NDRange(n,n));
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

