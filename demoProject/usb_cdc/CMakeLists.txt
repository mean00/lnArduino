

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

cmake_minimum_required(VERSION 3.0)
MESSAGE(STATUS "======================")
MESSAGE(STATUS "Starting usb test")
MESSAGE(STATUS "======================")

SET(LN_ENABLE_USBD True CACHE INTERNAL "")
SET(LN_ENABLE_I2C  FALSE CACHE INTERNAL "")
SET(LN_ENABLE_SPI  FALSE CACHE INTERNAL "")
SET(LN_USB_NB_CDC 1) # 1 CDC interfaces
SET(LN_USB_NB_HID 0) # No HID

include(../common_CMakeLists.cmake)

PROJECT(lnUsb_usbcdc NONE) # the NONE is very important !

enable_language(C CXX ASM) # this is important too!

#

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

#SET(LN_ENABLE_RUST ON CACHED "")

add_subdirectory(${AF_FOLDER})

include(${AF_FOLDER}/setup.cmake)
USE_LIBRARY(tinyUsb)        

include(${AF_FOLDER}/libraries/tinyUsb/tiny.cmake)

GENERATE_GD32_FIRMWARE(hello hello.cpp)    
        
