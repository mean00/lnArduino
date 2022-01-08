#=============================================================================#
# https://interrupt.memfault.com/blog/arm-cortexm-with-llvm-clang
MESSAGE(STATUS "Setting up GD32/arm cmake environment")
IF(NOT DEFINED LN_EXT)
SET(LN_EXT arm_gd32fx CACHE INTERNAL "")
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
IF(NOT EXISTS "${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${PLATFORM_TOOLCHAIN_SUFFIX}")
   FATAL_BANNER( "!! PLATFORM_TOOLCHAIN_PATH does not point to a valid toolchain ( ${PLATFORM_PREFIX}gcc....)!! (${PLATFORM_TOOLCHAIN_PATH})")
ENDIF()
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
# We use gcc linker
set(CMAKE_LINKER  ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}g++${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)

# dont try to create a shared lib, it will not work
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

MESSAGE(STATUS "GD32 C   compiler ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "GD32 C++ compiler ${CMAKE_CXX_COMPILER}")



# M3 or M4 ?

IF( "${LN_MCU}" STREQUAL "M3")
    SET(GD32_MCU "  -mcpu=cortex-m3 -mthumb  -march=armv7-m ")
ELSE()
    IF( "${LN_MCU}" STREQUAL "M4")
        SET(GD32_MCU "-mcpu=cortex-m4  -mfloat-abi=hard -mfpu=fpv4-sp-d16  -mthumb -DLN_USE_FPU=1")        
     ELSE()
         MESSAGE(FATAL_ERROR "Unsupported MCU : only M3 is supported (works for M0+) : ${LN_MCU}")
     ENDIF()
ENDIF()

#____________________________
# CLANG LINK USING GCC LIBS
#____________________________
SET(LN_CLANG_SYSROOT /home/fx/Arduino_stm32/arm-gcc-2020q4/bin/../arm-none-eabi CACHE INTERNAL "")
SET(LN_CLANG_LIBC /home/fx/Arduino_stm32/arm-gcc-2020q4/bin/../lib/gcc/arm-none-eabi/10.2.1/thumb/v7-m/nofp/libgcc.a CACHE INTERNAL "")
SET(LN_CLANG_MULTILIB thumb/v7-m/nofp CACHE INTERNAL "")

SET(G32_DEBUG_FLAGS "-g3 ${LN_LTO}  -Oz " CACHE INTERNAL "")

SET(GD32_LD_EXTRA "  -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align " CACHE INTERNAL "")
#
SET(GD32_C_FLAGS  "${GD32_SPECS}  ${PLATFORM_C_FLAGS} ${G32_DEBUG_FLAGS}  --sysroot=${LN_CLANG_SYSROOT} --target=arm-none-eabi -DLN_ARCH=LN_ARCH_ARM   ${GD32_BOARD_FLAG}  ${GD32_MCU}" CACHE INTERNAL "")
SET(CMAKE_C_FLAGS "${GD32_C_FLAGS}" CACHE INTERNAL "")
SET(CMAKE_ASM_FLAGS "${GD32_C_FLAGS}" CACHE INTERNAL "")
SET(CMAKE_CXX_FLAGS "${GD32_C_FLAGS}  -fno-rtti -fno-exceptions -fno-threadsafe-statics" CACHE INTERNAL "") 
#
SET(GD32_LD_FLAGS "-nostdlib ${GD32_SPECS} ${GD32_MCU} ${GD32_LD_EXTRA}" CACHE INTERNAL "")
SET(GD32_LD_LIBS "-lm -lc -lgcc" CACHE INTERNAL "")
#
SET(CLANG_LINKER_OPT "-Xlinker -no-enum-size-warning")  
#
set(CMAKE_CXX_LINK_EXECUTABLE    "<CMAKE_LINKER>   <CMAKE_CXX_LINK_FLAGS>  <LINK_FLAGS> ${LN_LTO} -lgcc -Xlinker -print-memory-usage   -Wl,--start-group  <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group  -Wl,-Map,<TARGET>.map   -o <TARGET> ${GD32_LD_FLAGS} ${GD32_LD_LIBS} ${CLANG_LINKER_OPT}" CACHE INTERNAL "")
#set(CMAKE_CXX_LINK_EXECUTABLE    "<CMAKE_LINKER>  -B ${PLATFORM_CLANG_PATH} <CMAKE_CXX_LINK_FLAGS>  <LINK_FLAGS> ${LN_LTO} -lgcc -Xlinker  -Xlinker -print-memory-usage   -Wl,--start-group  <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group  -Wl,-Map,<TARGET>.map   -o <TARGET> ${GD32_LD_FLAGS} ${GD32_LD_LIBS}" CACHE INTERNAL "")
#set(CMAKE_CXX_LINK_EXECUTABLE    "<CMAKE_LINKER>  -B ${PLATFORM_CLANG_PATH}   -Wl,-fuse-ld=lld${PLATFORM_CLANG_VERSION}${TOOLCHAIN_SUFFIX} <CMAKE_CXX_LINK_FLAGS>  <LINK_FLAGS> ${LN_LTO} -lgcc   -Xlinker -print-memory-usage   -Wl,--start-group  <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group  -Wl,-Map,<TARGET>.map   -o <TARGET> ${GD32_LD_FLAGS} ${GD32_LD_LIBS}" CACHE INTERNAL "")
SET(CMAKE_EXECUTABLE_SUFFIX_C .elf CACHE INTERNAL "")
SET(CMAKE_EXECUTABLE_SUFFIX_CXX .elf CACHE INTERNAL "")

include_directories(${ARDUINO_GD32_FREERTOS}/legacy/boards/${GD32_BOARD}/)




MESSAGE(STATUS "MCU Architecture ${LN_ARCH}")
MESSAGE(STATUS "MCU Type         ${LN_MCU}")
MESSAGE(STATUS "MCU Speed        ${LN_MCU_SPEED}")

ENDIF(NOT DEFINED LN_EXT)
