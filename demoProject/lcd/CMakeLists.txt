
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/Arduino_gd32_freeRTOS/)

SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.0)

PROJECT(Hello_gd32_freetos C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        

add_subdirectory(${AF_FOLDER}/libraries/ST7735)
include_directories(${AF_FOLDER}/libraries/ST7735)        


GENERATE_GD32_FIRMWARE(hello hello.cpp ${AMG})         
target_link_libraries(hello ST7735)
        
