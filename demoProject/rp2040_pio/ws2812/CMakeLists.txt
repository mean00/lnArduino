#-----------------------------------------------------------------------------
#
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
MESSAGE(STATUS "==================")
MESSAGE(STATUS "Starting pio_ws2812")
MESSAGE(STATUS "==================")

include(../common_CMakeLists.cmake)

project(PIO_SW2812 NONE)
enable_language(C CXX ASM)
#____________________
# Options
#____________________
include_directories( ${CMAKE_SOURCE_DIR} )
add_subdirectory(${AF_FOLDER})
include(${AF_FOLDER}/setup.cmake)
RP_PIO_GENERATE( ${CMAKE_SOURCE_DIR}/ws.pio ${CMAKE_BINARY_DIR}/ws.h)
GENERATE_GD32_FIRMWARE(pio_ws2812 main_pio.cpp pio_ws2812.cpp ${CMAKE_BINARY_DIR}/ws.h)

