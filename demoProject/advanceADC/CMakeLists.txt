cmake_minimum_required(VERSION 3.0)

include(../common_CMakeLists.cmake)
PROJECT(lnBasicADC C CXX ASM)

PROJECT(lnAdvanceADC C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})

include(${AF_FOLDER}/setup.cmake)
        
SET( NVM ${AF_FOLDER}/libraries/NeVerMind)
include_directories(${NVM})
include_directories(${NVM}/gd32)

SET(NVM_SRC ${NVM}/nvmCore.cpp  ${NVM}/gd32/nvm_gd32.cpp)        
        
GENERATE_GD32_FIRMWARE(hello hello.cpp ${NVM_SRC})        

        
