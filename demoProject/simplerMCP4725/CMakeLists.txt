cmake_minimum_required(VERSION 3.0)

include(../common_CMakeLists.cmake)


PROJECT(MCP4725 C CXX ASM)

SET(LN_ENABLE_I2C  ON CACHE INTERNAL "")

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        
USE_LIBRARY(simplerMCP4725)
GENERATE_GD32_FIRMWARE(mcp4725 hello.cpp )       
        
