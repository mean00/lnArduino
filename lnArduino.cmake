#=============================================================================#
# Author: Tomasz Bogdal (QueezyTheGreat)
# Home:   https://github.com/queezythegreat/arduino-cmake
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#=============================================================================#
IF(NOT DEFINED LN_ARCH)
    MESSAGE(STATUS "Architecture not defined, reverting to RISCV")
    SET(LN_ARCH "RISCV")
ENDIF(NOT DEFINED LN_ARCH)

STRING(TOUPPER ${LN_ARCH} LN_ARCH)
IF(LN_ARCH STREQUAL "RISCV")
    include(${CMAKE_CURRENT_LIST_DIR}/toolchain_riscv_gd32fx.cmake)
ELSE()
    IF(LN_ARCH STREQUAL "ARM")
        IF(USE_CLANG)
                include(${CMAKE_CURRENT_LIST_DIR}/toolchain_arm_gd32fx_clang.cmake)
        ELSE()
                include(${CMAKE_CURRENT_LIST_DIR}/toolchain_arm_gd32fx.cmake)
        ENDIF()
    ELSE()
        MESSAGE(FATAL_ERROR "LN_ARCH UNSUPPORTED, SET IT TO EITHER RISCV or ARM")
    ENDIF()
ENDIF()

SET(CMAKE_C_COMPILER_FORCED TRUE CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_FORCED TRUE CACHE INTERNAL "")
