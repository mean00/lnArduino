
cmake_minimum_required(VERSION 3.0)

include(../common_CMakeLists.cmake)

PROJECT(Hello_gd32_freetos C CXX ASM)
include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        

USE_LIBRARY(simplerAD9833)


GENERATE_GD32_FIRMWARE(hello hello.cpp ${AMG})         

        
