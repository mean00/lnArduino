#=============================================================================#
MESSAGE(STATUS "Setting up CH32V3x riscv cmake environment")
IF(NOT DEFINED LN_EXT)
    SET(LN_EXT riscv_ch32v3x CACHE INTERNAL "")
    include(${CMAKE_CURRENT_LIST_DIR}/../platformConfig.cmake)
    SET(LN_TOOLCHAIN_EXT  riscv_ch32v3x CACHE INTERNAL "")

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
    SET(GD32_BOARD_FLAG    ""  ) 
    SET(GD32_BOARD       ch32v3x CACHE INTERNAL "")

    IF(NOT DEFINED LN_MCU_SPEED)
        SET(LN_MCU_SPEED 72000000)
    ENDIF()
    # Set default value
    IF(NOT LN_MCU_RAM_SIZE)
        MESSAGE(STATUS "Ram size not set, using default")
        SET(LN_MCU_RAM_SIZE 64 )
    ENDIF(NOT LN_MCU_RAM_SIZE)
    IF(NOT LN_MCU_FLASH_SIZE)
        MESSAGE(STATUS "Flash size not set, using default")
        SET(LN_MCU_FLASH_SIZE 256 )
    ENDIF(NOT LN_MCU_FLASH_SIZE)
    IF(NOT LN_MCU_EEPROM_SIZE)
        MESSAGE(STATUS "NVME size not set, using default")
        SET(LN_MCU_EEPROM_SIZE 4 )
    ENDIF(NOT LN_MCU_EEPROM_SIZE)
    IF(NOT LN_MCU_STATIC_RAM)
        MESSAGE(STATUS "Static ram size not set, using default")
        SET(LN_MCU_STATIC_RAM 6 )
    ENDIF(NOT LN_MCU_STATIC_RAM)

    #
    SET(LN_MCU_RAM_SIZE ${LN_MCU_RAM_SIZE} CACHE INTERNAL "" FORCE)
    SET(LN_MCU_FLASH_SIZE ${LN_MCU_FLASH_SIZE} CACHE INTERNAL "" FORCE)
    SET(LN_MCU_EEPROM_SIZE ${LN_MCU_EEPROM_SIZE} CACHE INTERNAL "" FORCE)
    SET(LN_MCU_SPEED ${LN_MCU_SPEED} CACHE INTERNAL "" FORCE)

    #

    set(CMAKE_C_COMPILER   ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
    set(CMAKE_ASM_COMPILER ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}gcc${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
    set(CMAKE_CXX_COMPILER ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}g++${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
    set(CMAKE_OBJCOPY      ${PLATFORM_TOOLCHAIN_PATH}/${PLATFORM_PREFIX}objcopy${TOOLCHAIN_SUFFIX} CACHE PATH "" FORCE)
    # dont try to create a shared lib, it will not work
    SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

    MESSAGE(STATUS "CH32V3x C   compiler ${CMAKE_C_COMPILER}")
    MESSAGE(STATUS "CH32V3x C++ compiler ${CMAKE_CXX_COMPILER}")
    IF(LN_SPEC)
        SET(LN_SPEC "${LN_SPEC}" CACHE INTERNAL "" FORCE)
    ELSE(LN_SPEC)
        SET(LN_SPEC "nano" CACHE INTERNAL "" FORCE)
    ENDIF(LN_SPEC)
    SET(GD32_SPECS  "--specs=${LN_SPEC}.specs" CACHE INTERNAL "" FORCE)
    #
    SET(GD32_SPECS_C_FLAGS  "${GD32_SPECS_SPECS}  ${PLATFORM_C_FLAGS} -DLN_ARCH=LN_ARCH_RISCV -Werror=return-type  -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common ${GD32_BOARD_FLAG} -I${AF_FOLDER}/riscv_ch32v3x/")
    SET(CMAKE_C_FLAGS "${GD32_SPECS_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${GD32_SPECS_C_FLAGS}  -fno-rtti -fno-exceptions -fno-threadsafe-statics" ) 
    #
    SET(GD32_SPECS_LD_FLAGS "-nostdlib ${GD32_SPECS_SPECS}  -Wl,--traditional-format -Wl,--warn-common")
    SET(GD32_SPECS_LD_LIBS "-lm -lc -lgcc")

    #
    set(CMAKE_CXX_LINK_EXECUTABLE    "<CMAKE_CXX_COMPILER>   <CMAKE_CXX_LINK_FLAGS>  <LINK_FLAGS> -lgcc -Xlinker -print-memory-usage   -Wl,--start-group  <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group  -Wl,-Map,<TARGET>.map   -o <TARGET> ${GD32_LD_FLAGS} ${GD32_LD_LIBS}")
    SET(CMAKE_EXECUTABLE_SUFFIX_C .elf)
    SET(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)

    include_directories(${ARDUINO_GD32_FREERTOS}/legacy/boards/${GD32_BOARD}/)

    # Sees optimizaton >=2 are causing issues (???)

    ADD_DEFINITIONS("-g3  -Os ")
    #ADD_DEFINITIONS("-O0 ")


    MESSAGE(STATUS "MCU Architecture ${LN_ARCH}")
    MESSAGE(STATUS "MCU Type         ${LN_MCU}")
    MESSAGE(STATUS "MCU Speed        ${LN_MCU_SPEED}")
    MESSAGE(STATUS "MCU Flash Size   ${LN_MCU_FLASH_SIZE}")
    MESSAGE(STATUS "MCU Ram Size     ${LN_MCU_RAM_SIZE}")
    MESSAGE(STATUS "MCU Static RAM   ${LN_MCU_STATIC_RAM}")

ENDIF(NOT DEFINED LN_EXT)
