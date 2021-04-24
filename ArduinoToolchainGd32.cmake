#=============================================================================#
# Author: Tomasz Bogdal (QueezyTheGreat)
# Home:   https://github.com/queezythegreat/arduino-cmake
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#=============================================================================#
MESSAGE(STATUS "Setting up GD32 arduino cmake environment")
SET(ARDUINO_TOOLCHAIN_TOP ${CMAKE_SOURCE_DIR}/Arduino_gd32_freeRTOS CACHE INTERNAL "")

#
# Hardcoded for now
SET(PLATFORM_TOOLCHAIN_PATH /opt/gd32/toolchain/bin/)
SET(PLATFORM_PREFIX riscv32-unknown-elf-)
SET(PLATFORM_TOOLCHAIN_SUFFIX "") # .exe on windows
#
IF(NOT PLATFORM_TOOLCHAIN_PATH)
        MESSAGE(FATAL_ERROR "PLATFORM_TOOLCHAIN_PATH is not defined !")
ENDIF(NOT PLATFORM_TOOLCHAIN_PATH)
#


LIST(APPEND CMAKE_SYSTEM_PREFIX_PATH "${PLATFORM_TOOLCHAIN_PATH}")

FUNCTION(FATAL_BANNER msg)
    MESSAGE(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
    MESSAGE(STATUS "${msg}")
    MESSAGE(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
    MESSAGE(FATAL_ERROR "${msg}")
ENDFUNCTION(FATAL_BANNER msg)

IF(WIN32)
	SET(TOOLCHAIN_SUFFIX ".exe")
ENDIF(WIN32)
#
# Sanity check
#
IF(NOT EXISTS "${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${PLATFORM_TOOLCHAIN_SUFFIX}")
   FATAL_BANNER( "!! PLATFORM_TOOLCHAIN_PATH does not point to a valid toolchain ( ${PLATFORM_PREFIX}gcc....)!! (${PLATFORM_TOOLCHAIN_PATH})")
ENDIF()
#
# Setup toolchain for cross compilation
#
SET(CMAKE_SYSTEM_NAME Generic CACHE INTERNAL "")
SET(CMAKE_C_COMPILER_ID   "GNU" CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_ID "GNU" CACHE INTERNAL "")
set(CMAKE_C_COMPILER_WORKS      TRUE)
set(CMAKE_CXX_COMPILER_WORKS    TRUE)
#
SET(GD32_BOARD -DGD32VF103C_START) # Longan nano ?
#
SET(GD32_C_FLAGS  "-march=rv32imac -mabi=ilp32 -mcmodel=medlow -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common ${GD32_BOARD}")
SET(CMAKE_C_FLAGS ${GD32_C_FLAGS})
SET(CMAKE_CXXX_FLAGS ${GD32_C_FLAGS}  -fno-rtti -fno-exceptions )
#
SET(GD32MCU GD32VF103x8)
SET(GD32_LDSCRIPT ${ARDUINO_TOOLCHAIN_TOP}/Longduino_cmake/cores/arduino/GD32VF103_Firmware_Library/RISCV/env_Eclipse/${GD32MCU}.lds)
SET(GD32_LD_FLAGS "-nostdlib ")
SET(GD32_LD_LIBS "-lgd32Arduino -lstdc++ -lgd32 -lFreeRTOS -lgd32_overlay  -lc -lm -lgd32 -lgcc -lc  -lgcc -L${CMAKE_BINARY_DIR}/Arduino_gd32_freeRTOS  -L${CMAKE_BINARY_DIR}/Arduino_gd32_freeRTOS/src/ ")
#
set(CMAKE_C_COMPILER   ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_ASM_COMPILER ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}g++${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)

# dont try to create a shared lib, it will not work
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

MESSAGE(STATUS "GD32 C   compiler ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "GD32 C++ compiler ${CMAKE_CXX_COMPILER}")
#
set(CMAKE_CXX_LINK_EXECUTABLE    "<CMAKE_CXX_COMPILER>   <CMAKE_CXX_LINK_FLAGS>  <LINK_FLAGS>  -T ${GD32_LDSCRIPT} -lgcc -Wl,--start-group  <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group   -o <TARGET> ${GD32_LD_FLAGS} ${GD32_LD_LIBS}")
SET(CMAKE_EXECUTABLE_SUFFIX_C .elf)
SET(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Longduino_cmake/cores/arduino)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Longduino_cmake/cores/arduino/GD32VF103_Firmware_Library/GD32VF103_standard_peripheral)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Longduino_cmake/cores/arduino/GD32VF103_Firmware_Library/GD32VF103_standard_peripheral/Include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Longduino_cmake/cores/arduino/GD32VF103_Firmware_Library/RISCV/drivers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Longduino_cmake/variants/sipeed-longan-nano)
ADD_DEFINITIONS("-DGD32VF103V_EVAL")
ADD_DEFINITIONS("-g3")
