#=============================================================================#
# Author: Tomasz Bogdal (QueezyTheGreat)
# Home:   https://github.com/queezythegreat/arduino-cmake
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#=============================================================================#
MESSAGE(STATUS "Setting up GD32 riscv cmake environment")
IF(NOT DEFINED LN_EXT)
SET(LN_EXT riscv_gd32fx CACHE INTERNAL "")
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
SET(GD32_BOARD_FLAG      -DGD32VF103C_START) # Longan nano ?
SET(GD32_BOARD       sipeed-longan-nano CACHE INTERNAL "")

#

set(CMAKE_C_COMPILER   ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_ASM_COMPILER ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}g++${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)

# dont try to create a shared lib, it will not work
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

MESSAGE(STATUS "GD32 C   compiler ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "GD32 C++ compiler ${CMAKE_CXX_COMPILER}")

SET(GD32_SPECS  "--specs=nano.specs")
#
SET(GD32_C_FLAGS  "${GD32_SPECS}  ${PLATFORM_C_FLAGS} -DLN_ARCH=LN_ARCH_RISCV -Werror=return-type  -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common ${GD32_BOARD_FLAG}")
SET(CMAKE_C_FLAGS "${GD32_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${GD32_C_FLAGS}  -fno-rtti -fno-exceptions -fno-threadsafe-statics" ) 
#
SET(GD32_LD_FLAGS "-nostdlib ${GD32_SPECS}  -Wl,--traditional-format -Wl,--warn-common")
SET(GD32_LD_LIBS "-lm -lc -lgcc")
#-lgd32_overlay  -lgd32Arduino -lgd32 -lFreeRTOS -lgd32_lowlevel  -lc -lm -lgd32 -lgcc -lc  -lgcc 
#
set(CMAKE_CXX_LINK_EXECUTABLE    "<CMAKE_CXX_COMPILER>   <CMAKE_CXX_LINK_FLAGS>  <LINK_FLAGS> -lgcc -Xlinker -print-memory-usage   -Wl,--start-group  <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group  -Wl,-Map,<TARGET>.map   -o <TARGET> ${GD32_LD_FLAGS} ${GD32_LD_LIBS}")
SET(CMAKE_EXECUTABLE_SUFFIX_C .elf)
SET(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)

include_directories(${ARDUINO_GD32_FREERTOS}/legacy/boards/${GD32_BOARD}/)

# Sees optimizaton >=2 are causing issues (???)

ADD_DEFINITIONS("-g3  -Os ")
ENDIF()
