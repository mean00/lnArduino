#-----------------------------------------------------------------------------
# CMakeLists for rust_demo
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
MESSAGE(STATUS "======================")
MESSAGE(STATUS "Starting rust_demo")
MESSAGE(STATUS "======================")
#____________________
# Options
#____________________

SET(USE_CLANG      ON CACHE INTERNAL "")
SET(LN_ENABLE_RUST ON CACHE INTERNAL "")
SET(LN_ENABLE_I2C  FALSE CACHE INTERNAL "")
SET(LN_ENABLE_SPI  FALSE CACHE INTERNAL "")


#
SET(LN_USB_NB_CDC  0 CACHE INTERNAL "")
SET(LN_USB_NB_HID  0 CACHE INTERNAL "")
SET(LN_USB_DFU_RUNTIME 1 CACHE INTERNAL "")
SET(LN_ENABLE_USBD FALSE CACHE INTERNAL "")

#include(./build_default.cmake)

include(./mcuSelect.cmake)
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/lnArduino/)
SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/lnArduino.cmake)



PROJECT(lnADCTest NONE) # the NONE is very important !

enable_language(C CXX ASM) # this is important too!

#

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${AF_FOLDER})

include(${AF_FOLDER}/setup.cmake)

SET(EXTENSION "_${LN_MCU}")


include_directories(src)

#_______________________
# FPU
#_______________________
IF("${LN_ARCH}" STREQUAL "ARM")
    IF("${LN_MCU}" STREQUAL "M3")
        SET(FP_LIB qfpm3)
    ENDIF()
ENDIF()

#_______________________
#  Rust vs C++
#_______________________
SET(START_CODE  ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/rust_start.cpp )
#SET(CPP_CODE    ${CMAKE_CURRENT_SOURCE_DIR}/src/rs/cpp/lnI2CTask.cpp )
#


GENERATE_GD32_FIRMWARE(rust_demo ${START_CODE}  )
HASH_GD32_FIRMWARE( rust_demo )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/src/)
TARGET_LINK_LIBRARIES( rust_demo lnarduino_c_bindings )
TARGET_LINK_LIBRARIES( rust_demo rsPowerSupply )

# Summary
MESSAGE(STATUS "Configuration:")
MESSAGE(STATUS "\tBuild folder ${CMAKE_BINARY_DIR}")
MESSAGE(STATUS "\tUsing ${LN_MCU} based MCU at ${LN_MCU_SPEED} Hz")
MESSAGE(STATUS "\tUsing ${LN_MCU_FLASH_SIZE} kB of flash (small = ${USE_SMALLFOOTPRINT})")
MESSAGE(STATUS "\tUSB control enabled : ${LN_ENABLE_USBD}")


#
SET(EXTENSION ${EXTENSION}_RUST)
set_target_properties(rust_demo PROPERTIES OUTPUT_NAME rust_demo${EXTENSION})

MESSAGE(STATUS "\tFinal name is ${EXTENSION}")
