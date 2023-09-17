#=============================================================================#
# Author: Tomasz Bogdal (QueezyTheGreat)
# Home:   https://github.com/queezythegreat/arduino-cmake
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#=============================================================================#
OPTION(USE_SCAN_BUILD "Disable custom CC") 
IF(NOT DEFINED LN_ARCH)
    MESSAGE(STATUS "Architecture not defined, reverting to RISCV")
    SET(LN_ARCH "RISCV" CACHE INTERNAL "")
ENDIF()

STRING(TOUPPER ${LN_ARCH} LN_ARCH)
SET(LN_MCU_TOOLCHAIN_PREFIX "UNKNOWN")
IF(LN_ARCH STREQUAL "RISCV")
    IF("${LN_MCU}" STREQUAL "CH32V3x")
        SET(LN_MCU_TOOLCHAIN_PREFIX ${CMAKE_CURRENT_LIST_DIR}/mcus/riscv_ch32v3x/ CACHE INTERNAL "")
    ENDIF()
ENDIF()

IF(LN_ARCH STREQUAL "ARM")
    IF("${LN_MCU}" STREQUAL "RP2040")
        SET(LN_MCU_TOOLCHAIN_PREFIX ${CMAKE_CURRENT_LIST_DIR}/mcus/arm_rp2040/ CACHE INTERNAL "")
    ELSE()
        SET(LN_MCU_TOOLCHAIN_PREFIX ${CMAKE_CURRENT_LIST_DIR}/mcus/arm_gd32fx/ CACHE INTERNAL "")
    ENDIF()
ENDIF()

IF("${LN_MCU_TOOLCHAIN_PREFIX}" STREQUAL "UNKNOWN")
    MESSAGE(FATAL_ERROR "LN_ARCH UNSUPPORTED, SET IT TO EITHER RISCV or ARM")
ENDIF()

SET(LN_MCU_TOOLCHAIN_PREFIX ${LN_MCU_TOOLCHAIN_PREFIX} CACHE INTERNAL "")

IF(USE_CLANG)
            include(${LN_MCU_TOOLCHAIN_PREFIX}/toolchain_clang.cmake)
ELSE()
            include(${LN_MCU_TOOLCHAIN_PREFIX}/toolchain.cmake)
ENDIF()            

SET(CMAKE_C_COMPILER_FORCED TRUE CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_FORCED TRUE CACHE INTERNAL "")
