cmake_minimum_required(VERSION 3.13)
project(cpu_scheduler)

set(CMAKE_CXX_STANDARD 23)

add_executable(cpu_scheduler
    src/main.cpp
    src/simulator.cpp
)
