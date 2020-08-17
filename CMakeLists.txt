cmake_minimum_required(VERSION 3.17)
project(zurGBemu)

set(CMAKE_CXX_STANDARD 14)

add_executable(zurGBemu main.cpp CPU.hpp Memory.hpp Memory.cpp Rom.hpp Rom.cpp CPU.cpp)