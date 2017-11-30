cmake_minimum_required(VERSION 3.8)
project(auth_thesis_fhe_si)

set(CMAKE_CXX_STANDARD 11)
include_directories(/usr/local/include/FHESI)
include_directories(/usr/local/include/NTL)
set(SOURCE_FILES  helpfunctions.cpp  main11.cpp)


add_executable(auth_thesis_fhe_si ${SOURCE_FILES})

target_link_libraries(auth_thesis_fhe_si libfhesi.a libntl.a libgmp.a libm.a)
