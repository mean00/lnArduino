
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/Arduino_gd32_freeRTOS/)

SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/ArduinoToolchainGd32.cmake)

cmake_minimum_required(VERSION 3.0)

PROJECT(Longan_WS12812B C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        
USE_LIBRARY(WS2812B)
GENERATE_GD32_FIRMWARE(ws2812 hello.cpp )       
        
