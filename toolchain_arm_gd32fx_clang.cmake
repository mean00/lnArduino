#=============================================================================#
# https://interrupt.memfault.com/blog/arm-cortexm-with-llvm-clang
MESSAGE(STATUS "Setting up GD32/arm cmake environment")
IF(NOT DEFINED LN_EXT)
SET(LN_EXT arm_gd32fx CACHE INTERNAL "")
SET(LN_TOOLCHAIN_EXT  arm_gd32fx_clang CACHE INTERNAL "")

include(${CMAKE_CURRENT_LIST_DIR}/../platformConfig.cmake)

IF(NOT PLATFORM_TOOLCHAIN_PATH)
        MESSAGE(FATAL_ERROR "PLATFORM_TOOLCHAIN_PATH is not defined in platformConfig.cmake !!")
ENDIF(NOT PLATFORM_TOOLCHAIN_PATH)
#
#SET(LN_LTO "-flto")
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
#
# Setup toolchain for cross compilation
#
SET(CMAKE_SYSTEM_NAME Generic CACHE INTERNAL "")
SET(CMAKE_C_COMPILER_ID   "Clang" CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_ID "Clang" CACHE INTERNAL "")
set(CMAKE_C_COMPILER_WORKS      TRUE)
set(CMAKE_CXX_COMPILER_WORKS    TRUE)
#
SET(GD32_BOARD       bluepill CACHE INTERNAL "")

# Speed

IF(NOT DEFINED LN_MCU_SPEED)
    SET(LN_MCU_SPEED 72000000)
ENDIF()
SET(LN_MCU_SPEED ${LN_MCU_SPEED} CACHE INTERNAL "")

set(CMAKE_C_COMPILER   ${PLATFORM_CLANG_PATH}/clang${PLATFORM_CLANG_VERSION}${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_ASM_COMPILER ${PLATFORM_CLANG_PATH}/clang${PLATFORM_CLANG_VERSION}${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER ${PLATFORM_CLANG_PATH}/clang++${PLATFORM_CLANG_VERSION}${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
set(CMAKE_OBJCOPY      ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}objcopy${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
# We use gcc linker
set(CMAKE_LINKER  ${CMAKE_CXX_COMPILER} CACHE PATH "" FORCE)

# dont try to create a shared lib, it will not work
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

MESSAGE(STATUS "GD32 C   compiler ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "GD32 C++ compiler ${CMAKE_CXX_COMPILER}")



# M3 or M4 ?

IF( "${LN_MCU}" STREQUAL "M3")
    SET(GD32_MCU "  -mfloat-abi=soft --target=armv7m-none-eabi  ")
    SET(LN_CLANG_SYSROOT "${PLATFORM_CLANG_PATH}/../lib/clang-runtimes/armv7m_soft_nofp/" CACHE INTERNAL "")
    SET(GD32_LIBC " " CACHE INTERNAL "")
    #SET(GD32_LIBC " -L${PLATFORM_CLANG_PATH}/../lib/clang-runtimes/armv7m_soft_nofp/lib" CACHE INTERNAL "")

ELSE()
    IF( "${LN_MCU}" STREQUAL "M4")
        SET(LN_CLANG_SYSROOT "${PLATFORM_CLANG_PATH}/../lib/clang-runtimes/armv7em_hard_fpv4_sp_d16/lib" CACHE INTERNAL "")
        SET(GD32_LIBC " " CACHE INTERNAL "")
        SET(GD32_MCU "-mcpu=cortex-m4 --target=armv7m-none-eabi   -mfloat-abi=hard -mfpu=fpv4-sp-d16  -DLN_USE_FPU=1")        
     ELSE()
         MESSAGE(FATAL_ERROR "Unsupported MCU : only M3 is supported (works for M0+) : ${LN_MCU}")
     ENDIF()
ENDIF()

#____________________________
# CLANG LINK USING GCC LIBS
#____________________________
#SET(LN_CLANG_LIBC /home/fx/Arduino_stm32/arm-gcc-2020q4/bin/../lib/gcc/arm-none-eabi/10.2.1/thumb/v7-m/nofp/libgcc.a CACHE INTERNAL "")
#SET(LN_CLANG_MULTILIB thumb/v7-m/nofp CACHE INTERNAL "")

SET(G32_DEBUG_FLAGS "-g3 ${LN_LTO}  -Oz " CACHE INTERNAL "")

SET(GD32_LD_EXTRA "  -Wl,--unresolved-symbols=report-all -Wl,--warn-common  " CACHE INTERNAL "")
#
SET(GD32_C_FLAGS  "${GD32_SPECS}  ${PLATFORM_C_FLAGS} ${G32_DEBUG_FLAGS}  --sysroot=${LN_CLANG_SYSROOT} --target=arm-none-eabi -DLN_ARCH=LN_ARCH_ARM   ${GD32_BOARD_FLAG}  ${GD32_MCU}" CACHE INTERNAL "")
SET(CMAKE_C_FLAGS "${GD32_C_FLAGS}" CACHE INTERNAL "")
SET(CMAKE_ASM_FLAGS "${GD32_C_FLAGS}" CACHE INTERNAL "")
SET(CMAKE_CXX_FLAGS "${GD32_C_FLAGS}  -fno-rtti -fno-exceptions -fno-threadsafe-statics" CACHE INTERNAL "") 
#
SET(GD32_LD_FLAGS " -fuse-ld=lld -nostdlib ${GD32_SPECS}  ${GD32_MCU}  ${GD32_LD_EXTRA}  ${GD32_LIBC}" CACHE INTERNAL "")
SET(GD32_LD_LIBS "-lm -lc " CACHE INTERNAL "")
#
SET(CLANG_LINKER_OPT "${LN_CLANG_SYSROOT}/lib/libclang_rt.builtins-armv7m.a")  
#
set(CMAKE_CXX_LINK_EXECUTABLE    "<CMAKE_LINKER>   <CMAKE_CXX_LINK_FLAGS>  <LINK_FLAGS> ${LN_LTO} --sysroot=${LN_CLANG_SYSROOT}     -Wl,--start-group  <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group  -Wl,-Map,<TARGET>.map   -o <TARGET> ${GD32_LD_FLAGS} ${GD32_LD_LIBS} ${CLANG_LINKER_OPT}" CACHE INTERNAL "")
SET(CMAKE_EXECUTABLE_SUFFIX_C .elf CACHE INTERNAL "")
SET(CMAKE_EXECUTABLE_SUFFIX_CXX .elf CACHE INTERNAL "")

include_directories(${ARDUINO_GD32_FREERTOS}/legacy/boards/${GD32_BOARD}/)




MESSAGE(STATUS "MCU Architecture ${LN_ARCH}")
MESSAGE(STATUS "MCU Type         ${LN_MCU}")
MESSAGE(STATUS "MCU Speed        ${LN_MCU_SPEED}")

ENDIF(NOT DEFINED LN_EXT)
