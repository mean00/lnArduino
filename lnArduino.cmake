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
ENDIF(NOT DEFINED LN_ARCH)

STRING(TOUPPER ${LN_ARCH} LN_ARCH)
IF(LN_ARCH STREQUAL "RISCV")
    IF("${LN_MCU}" STREQUAL "CH32V3x")
        IF(USE_CLANG)
            include(${CMAKE_CURRENT_LIST_DIR}/mcus/riscv_ch32v3x/toolchain_riscv_ch32v3x_clang.cmake)
        ELSE()
            include(${CMAKE_CURRENT_LIST_DIR}/mcus/riscv_ch32v3x/toolchain_riscv_ch32v3x.cmake)
        ENDIF()
    ELSE()
        include(${CMAKE_CURRENT_LIST_DIR}/mcus/riscv_gd32fx/toolchain_riscv_gd32fx.cmake)
    ENDIF()
ELSE()
    IF(LN_ARCH STREQUAL "ARM")
        IF(USE_CLANG)
                include(${CMAKE_CURRENT_LIST_DIR}/mcus/arm_gd32fx/toolchain_arm_gd32fx_clang.cmake)
        ELSE()
                include(${CMAKE_CURRENT_LIST_DIR}/mcus/arm_gd32fx/toolchain_arm_gd32fx.cmake)
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR "LN_ARCH UNSUPPORTED, SET IT TO EITHER RISCV or ARM")
    ENDIF()
ENDIF()

SET(CMAKE_C_COMPILER_FORCED TRUE CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_FORCED TRUE CACHE INTERNAL "")
