#-----------------------------------------------------------------------------
# CMakeLists for lnPowerSupply
#-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
MESSAGE(STATUS "======================")
MESSAGE(STATUS "Starting lnPowerSupply")
MESSAGE(STATUS "======================")
#____________________
# Options
#____________________

SET(USE_CLANG      ON CACHE INTERNAL "")
#SET(LN_ENABLE_RUST ON CACHE INTERNAL "")
SET(LN_ENABLE_I2C  ON CACHE INTERNAL "")
SET(LN_ENABLE_SPI  ON CACHE INTERNAL "")

SET(LN_MCU_SPEED 72000000  CACHE INTERNAL "")

#IF(TRUE)
#SET(LN_LTO "-flto" CACHE INTERNAL "")
#SET(LN_LTO_RUST_FLAGS ="-Clinker-plugin-lto=yes " CACHE INTERNAL "")
#ELSE()
#SET(LN_LTO_RUST_FLAGS ="-Clinker-plugin-lto=no " CACHE INTERNAL "")
#ENDIF()
#
SET(LN_USB_NB_CDC  0 CACHE INTERNAL "")
SET(LN_USB_NB_HID  0 CACHE INTERNAL "")
SET(LN_USB_DFU_RUNTIME 1 CACHE INTERNAL "")

OPTION(USE_USB                  "Enable USB support"                                FALSE)
OPTION(USE_GD32F3               "Build for GD32F303"                                FALSE)




SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/lnArduino/)
include(./mcuSelect.cmake)
SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/lnArduino.cmake)



PROJECT(lnPowerSupply NONE) # the NONE is very important !

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
# FPU
#_______________________
IF("${LN_ARCH}" STREQUAL "RISCV")
    SET(FP_LIB rvfp)
ELSEIF("${LN_ARCH}" STREQUAL "ARM")
    IF("${LN_MCU}" STREQUAL "M3")
        SET(FP_LIB qfpm3)
    ENDIF("${LN_MCU}" STREQUAL "M3")
ENDIF("${LN_ARCH}" STREQUAL "RISCV")

#_______________________
#  Rust vs C++
#_______________________
SET(START_CODE  src/cpp/main.cpp )    
#




GENERATE_GD32_FIRMWARE(lnPowerSupply ${START_CODE} ${CPP_CODE}  lnArduino/src/lnFreeRTOSDebug.cpp )
IF(LN_ENABLE_USBD)
    TARGET_LINK_LIBRARIES( lnPowerSupply  gd32_usb_usbd)
ENDIF()

IF(LN_ENABLE_RUST)
    TARGET_LINK_LIBRARIES( lnPowerSupply  demo lnarduino_c_bindings )
ENDIF(LN_ENABLE_RUST)




# Summary
MESSAGE(STATUS "Configuration:")
MESSAGE(STATUS "\tBuild folder ${CMAKE_BINARY_DIR}")
MESSAGE(STATUS "\tUsing ${LN_MCU} based MCU at ${LN_MCU_SPEED} Hz")
MESSAGE(STATUS "\tUsing ${LN_MCU_FLASH_SIZE} kB of flash (small = ${USE_SMALLFOOTPRINT})")
MESSAGE(STATUS "\tUSB control enabled : ${LN_ENABLE_USBD}")
#
set_target_properties(lnPowerSupply PROPERTIES OUTPUT_NAME lnPowerSupply${EXTENSION})

MESSAGE(STATUS "\tFinal name is ${EXTENSION}")
