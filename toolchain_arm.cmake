#=============================================================================#
# Author: Tomasz Bogdal (QueezyTheGreat)
# Home:   https://github.com/queezythegreat/arduino-cmake
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#=============================================================================#
MESSAGE(STATUS "Setting up GD32/arm cmake environment")
IF(NOT DEFINED LN_EXT)
SET(LN_EXT arm CACHE INTERNAL "")
include(${CMAKE_CURRENT_LIST_DIR}/../platformConfig.cmake)

IF(NOT PLATFORM_TOOLCHAIN_PATH)
        MESSAGE(FATAL_ERROR "PLATFORM_TOOLCHAIN_PATH is not defined in platformConfig.cmake !!")
ENDIF(NOT PLATFORM_TOOLCHAIN_PATH)
#


LIST(APPEND CMAKE_SYSTEM_PREFIX_PATH "${PLATFORM_TOOLCHAIN_PATH}")

FUNCTION(FATAL_BANNER msg)
    MESSAGE(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
    MESSAGE(STATUS "${msg}")
    MESSAGE(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
    MESSAGE(FATAL_ERROR "${msg}")
ENDFUNCTION(FATAL_BANNER msg)

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
SET(GD32_BOARD       bluepill CACHE INTERNAL "")

#

set(CMAKE_C_COMPILER   ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_ASM_COMPILER ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}g++${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)

# dont try to create a shared lib, it will not work
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

MESSAGE(STATUS "GD32 C   compiler ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "GD32 C++ compiler ${CMAKE_CXX_COMPILER}")

SET(GD32_SPECS  "--specs=nano.specs")

IF( "${LN_MCU}" STREQUAL "M3")
    SET(GD32_MCU "  -mcpu=cortex-m3 -mthumb  -march=armv7-m ")
ELSE()
    MESSAGE(FATAL_ERROR "Unsupported MCU : only M3 is supported (works for M0+) : ${LN_MCU}")
ENDIF()


SET(GD32_LD_EXTRA "  -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align " CACHE INTERNAL "")
#
SET(GD32_C_FLAGS  "${GD32_SPECS}  ${PLATFORM_C_FLAGS} -DLN_ARCH=LN_ARCH_ARM  -Werror=return-type -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common ${GD32_BOARD_FLAG}  ${GD32_MCU}" CACHE INTERNAL "")
SET(CMAKE_C_FLAGS "${GD32_C_FLAGS}" CACHE INTERNAL "")
SET(CMAKE_ASM_FLAGS "${GD32_C_FLAGS}" CACHE INTERNAL "")
SET(CMAKE_CXX_FLAGS "${GD32_C_FLAGS}  -fno-rtti -fno-exceptions" CACHE INTERNAL "") 
#
SET(GD32_LD_FLAGS "-nostdlib ${GD32_SPECS} ${GD32_MCU} ${GD32_LD_EXTRA}" CACHE INTERNAL "")
SET(GD32_LD_LIBS "-lm -lc -lgcc" CACHE INTERNAL "")
#
set(CMAKE_CXX_LINK_EXECUTABLE    "<CMAKE_CXX_COMPILER>   <CMAKE_CXX_LINK_FLAGS>  <LINK_FLAGS> -lgcc -Xlinker -print-memory-usage   -Wl,--start-group  <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group  -Wl,-Map,<TARGET>.map   -o <TARGET> ${GD32_LD_FLAGS} ${GD32_LD_LIBS}" CACHE INTERNAL "")
SET(CMAKE_EXECUTABLE_SUFFIX_C .elf CACHE INTERNAL "")
SET(CMAKE_EXECUTABLE_SUFFIX_CXX .elf CACHE INTERNAL "")

include_directories(${ARDUINO_GD32_FREERTOS}/legacy/boards/${GD32_BOARD}/)


ADD_DEFINITIONS("-g3 -O1 ")


ENDIF(NOT DEFINED LN_EXT)