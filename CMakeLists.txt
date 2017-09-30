cmake_minimum_required(VERSION 3.8)
project(auth_thesis_fhe_si)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp helpfunctions.h helpfunctions.cpp)
add_executable(auth_thesis_fhe_si ${SOURCE_FILES})

include_directories("/Users/geo7/software/fhe-si/")