
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/Arduino_gd32_freeRTOS/)

SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.0)

PROJECT(Hello_gd32_freetos C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})

include(${AF_FOLDER}/setup.cmake)
ADD_EXECUTABLE(hello hello.cpp )
ADD_DEPENDENCIES(hello gd32_overlay gd32 gd32_overlay FreeRTOS gd32Arduino gd32_lowlevel)
