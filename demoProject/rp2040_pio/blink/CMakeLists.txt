#-----------------------------------------------------------------------------
#
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
MESSAGE(STATUS "==================")
MESSAGE(STATUS "Starting pio_blink")
MESSAGE(STATUS "==================")

include(../common_CMakeLists.cmake)

project(PIO_BLINK NONE)
enable_language(C CXX ASM)
#____________________
# Options
#____________________
include_directories( ${CMAKE_SOURCE_DIR} )
add_subdirectory(${AF_FOLDER})
include(${AF_FOLDER}/setup.cmake)
GENERATE_GD32_FIRMWARE(pio_blink main_pio.cpp pio_blink.cpp )

