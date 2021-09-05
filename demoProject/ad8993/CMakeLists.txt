
cmake_minimum_required(VERSION 3.0)

include(../common_CMakeLists.cmake)

PROJECT(Hello_gd32_freetos NONE) # the NONE is very important !

enable_language(C CXX ASM) # this is important too!
include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        

USE_LIBRARY(simplerAD8933)


GENERATE_GD32_FIRMWARE(hello hello.cpp ${AMG})         

        
