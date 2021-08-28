#
#
cmake_minimum_required(VERSION 3.0)

include(../common_CMakeLists.cmake)

PROJECT(RotaryEncoder C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})

include(${AF_FOLDER}/setup.cmake)       
      
SET( Rotary ${AF_FOLDER}/libraries/RotaryEncoder )
INCLUDE_DIRECTORIES( ${Rotary} )
ADD_SUBDIRECTORY( ${Rotary} )

        
GENERATE_GD32_FIRMWARE(hello hello.cpp)   
TARGET_LINK_LIBRARIES(hello RotaryEncoder)
        
