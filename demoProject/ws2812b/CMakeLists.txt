
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/Arduino_gd32_freeRTOS/)

SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.0)

PROJECT(Longa&n_WS12812B C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        


GENERATE_GD32_FIRMWARE(hello hello.cpp ${AMG})         
        
