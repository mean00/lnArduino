
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/Arduino_gd32_freeRTOS/)

SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.0)

PROJECT(Hello_gd32_freetos C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        
SET(ST7735_DRIVER ${AF_FOLDER}/libraries/simplerST7735/platformExample/longanNano/gd32ST7735.cpp)
include_directories(ST7735_DRIVER ${AF_FOLDER}/libraries/simplerST7735/platformExample/longanNano/)


USE_LIBRARY(simplerST7735)


GENERATE_GD32_FIRMWARE(hello hello.cpp ${AMG})         

        
