cmake_minimum_required(VERSION 3.12)
project(Git)

set(CMAKE_CXX_STANDARD 11)
include_directories(files)

add_executable(Git main.cpp
        files/Auxilaries.h
        files/Complex.h
        files/MtmVec.h
        files/MtmMat.h
        files/MtmExceptions.h
        files/MtmMatSq.h
        files/MtmMatTriag.h
        files/Complex.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -o main")