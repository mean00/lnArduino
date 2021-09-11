
MACRO(GENERATE_GD32_FIRMWARE target)

  IF(NOT LN_MCU_RAM_SIZE)
        SET(LN_MCU_RAM_SIZE 32)
    ENDIF(NOT LN_MCU_RAM_SIZE)
    IF(NOT LN_MCU_FLASH_SIZE)
        SET(LN_MCU_FLASH_SIZE 128)
    ENDIF(NOT LN_MCU_FLASH_SIZE)
    IF(NOT LN_MCU_EEPROM_SIZE)
        SET(LN_MCU_EEPROM_SIZE 4)
    ENDIF(NOT LN_MCU_EEPROM_SIZE)

   configure_file( "${AF_FOLDER}/legacy/boards/sipeed-longan-nano/ld.lds.in" "${CMAKE_BINARY_DIR}/linker_script.lds" @ONLY)

    ADD_EXECUTABLE(${target}  ${AF_FOLDER}/riscv_gd32fx/start.S ${ARGN})
    TARGET_LINK_LIBRARIES(${target} ${USED_LIBS} ) # duplicates are NOT a mistake !
    TARGET_LINK_LIBRARIES(${target} embeddedPrintf gd32_overlay gd32Arduino   FreeRTOS  gd32_lowlevel c  c gcc ) # dupicates are NOT a mistake !
    TARGET_LINK_OPTIONS(${target}  PRIVATE "-T${CMAKE_BINARY_DIR}/linker_script.lds" )
ENDMACRO(GENERATE_GD32_FIRMWARE target)

MACRO(USE_LIBRARY lib)
    add_subdirectory(${AF_FOLDER}/libraries/${lib})
    include_directories(${AF_FOLDER}/libraries/${lib})
    LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
