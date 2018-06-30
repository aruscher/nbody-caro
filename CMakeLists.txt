cmake_minimum_required(VERSION 3.10)
project(nbody_caro)

# Handle OpenCL
find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

set(CMAKE_CXX_STANDARD 11)
add_executable(nbody_caro main.cpp Body.cpp Body.h Simulator.cpp Simulator.h NBodySystem.cpp NBodySystem.h)
target_include_directories (nbody_caro PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (nbody_caro ${OpenCL_LIBRARY})
