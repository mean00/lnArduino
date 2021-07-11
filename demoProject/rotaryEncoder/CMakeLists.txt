#
#
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/Arduino_gd32_freeRTOS/)

include(./mcuSelect.cmake)
SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/lnArduino.cmake)
cmake_minimum_required(VERSION 3.0)

PROJECT(RotaryEncoder C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})

include(${AF_FOLDER}/setup.cmake)       
      
SET( Rotary ${AF_FOLDER}/libraries/RotaryEncoder )
INCLUDE_DIRECTORIES( ${Rotary} )
ADD_SUBDIRECTORY( ${Rotary} )

        
GENERATE_GD32_FIRMWARE(hello hello.cpp)   
TARGET_LINK_LIBRARIES(hello RotaryEncoder)
        
