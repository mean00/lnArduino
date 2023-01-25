#-----------------------------------------------------------------------------
# CMakeLists for lnProject
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
MESSAGE(STATUS "======================")
MESSAGE(STATUS "Starting lnProject")
MESSAGE(STATUS "======================")

#____________________
# Options
#____________________

OPTION(USE_USB                  "Enable USB support"                                FALSE)
OPTION(USE_GD32F3               "Build for GD32F303"                                FALSE)
OPTION(USE_HW_FPU               "Enable HW FPU build"                               FALSE)


SET(USE_HW_FPU     TRUE CACHE INTERNAL "")
SET(USE_CLANG      ON CACHE INTERNAL "")
#SET(LN_ENABLE_RUST ON CACHE INTERNAL "")
SET(LN_ENABLE_I2C  OFF CACHE INTERNAL "")
SET(LN_ENABLE_SPI  OFF CACHE INTERNAL "")

SET(LN_MCU_SPEED 72000000  CACHE INTERNAL "")

IF(LN_ENABLE_RUST)
       SET(LN_LTO "-flto" CACHE INTERNAL "")
       SET(LN_LTO_RUST_FLAGS ="-Clinker-plugin-lto=yes " CACHE INTERNAL "")
ELSE(LN_ENABLE_RUST)    
       SET(LN_LTO "-flto" CACHE INTERNAL "")
ENDIF()
#

#SET(LN_LTO_RUST_FLAGS ="-Clinker-plugin-lto=yes " CACHE INTERNAL "")
#ELSE()
#SET(LN_LTO_RUST_FLAGS ="-Clinker-plugin-lto=no " CACHE INTERNAL "")
#ENDIF()
#
SET(LN_USB_NB_CDC  0 CACHE INTERNAL "")
SET(LN_USB_NB_HID  0 CACHE INTERNAL "")
SET(LN_USB_DFU_RUNTIME 1 CACHE INTERNAL "")





SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/lnArduino/)
include(./mcuSelect.cmake)
SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/lnArduino.cmake)



PROJECT(HELLO NONE) # the NONE is very important !

enable_language(C CXX ASM) # this is important too!

#

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${AF_FOLDER})

include(${AF_FOLDER}/setup.cmake)

SET(EXTENSION "_${LN_MCU}")
math(EXPR MCU_SPEED_M "${LN_MCU_SPEED}/1000000")
SET(EXTENSION "${EXTENSION}_${MCU_SPEED_M}M")


include_directories(src)

#_______________________
#  Rust vs C++
#_______________________
SET(START_CODE  src/cpp/main.cpp )    
#

USE_LIBRARY(NeVerMind)

GENERATE_GD32_FIRMWARE(lnProject ${START_CODE} ${CPP_CODE}  lnArduino/src/lnFreeRTOSDebug.cpp )
# Summary
MESSAGE(STATUS "Configuration:")
MESSAGE(STATUS "\tBuild folder ${CMAKE_BINARY_DIR}")
MESSAGE(STATUS "\tUsing ${LN_MCU} based MCU at ${LN_MCU_SPEED} Hz")
MESSAGE(STATUS "\tUsing ${LN_MCU_FLASH_SIZE} kB of flash (small = ${USE_SMALLFOOTPRINT})")
MESSAGE(STATUS "\tUSB control enabled : ${LN_ENABLE_USBD}")
#
set_target_properties(lnProject PROPERTIES OUTPUT_NAME lnProject${EXTENSION})

MESSAGE(STATUS "\tFinal name is ${EXTENSION}")
