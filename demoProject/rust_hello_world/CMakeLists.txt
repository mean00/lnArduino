
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.13)
MESSAGE(STATUS "======================")
MESSAGE(STATUS "Starting rust demo ")
MESSAGE(STATUS "======================")

#SET(LN_LTO "-flto"  CACHE INTERNAL "")
SET(LN_ENABLE_USBD  OFF CACHE INTERNAL "")
SET(LN_ENABLE_I2C   FALSE CACHE INTERNAL "")
SET(LN_ENABLE_SPI   FALSE CACHE INTERNAL "")
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/lnArduino)
SET(LN_ENABLE_RUST ON CACHED "")
include(./mcuSelect.cmake)

SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/lnArduino.cmake)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${AF_FOLDER}/cmake CACHE INTERNAL "")

PROJECT(lnRUST_DEMO NONE) # the NONE is very important !

enable_language(C CXX ASM) # this is important too!

add_subdirectory(${AF_FOLDER})

include(${AF_FOLDER}/setup.cmake)
GENERATE_GD32_FIRMWARE(lnDEMO src/demo.cpp)


ADD_SUBDIRECTORY(rust)
target_link_libraries(lnDEMO rust_demo)
