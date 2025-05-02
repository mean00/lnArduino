
MACRO(GENERATE_GD32_FIRMWARE target)

  IF(NOT LN_MCU_RAM_SIZE)
    SET(LN_MCU_RAM_SIZE 32 CACHE INTERNAL "")
  ENDIF(NOT LN_MCU_RAM_SIZE)
  IF(NOT LN_MCU_FLASH_SIZE)
    SET(LN_MCU_FLASH_SIZE 128 CACHE INTERNAL "")
  ENDIF(NOT LN_MCU_FLASH_SIZE)
  IF(NOT LN_MCU_EEPROM_SIZE)
    SET(LN_MCU_EEPROM_SIZE 4 CACHE INTERNAL "")
  ENDIF(NOT LN_MCU_EEPROM_SIZE)

  configure_file( "${LN_MCU_FOLDER}/boards/esp32c3/clang.lld.in" "${CMAKE_BINARY_DIR}/linker_script.ld" @ONLY)


  ADD_EXECUTABLE(${target}  ${LN_MCU_FOLDER}/start.S ${ARGN})
  TARGET_LINK_LIBRARIES(${target} ${USED_LIBS} ) # duplicates are NOT a mistake !
  TARGET_LINK_LIBRARIES(${target} lnArduino embeddedPrintf lnArduino_impl FreeRTOS     ) # dupicates are NOT a mistake !

  IF(LN_CUSTOM_LD_SCRIPT)
    SET(SCRIPT ${LN_CUSTOM_LD_SCRIPT} CACHE INTERNAL "")
  ELSE()
    SET(SCRIPT "${CMAKE_BINARY_DIR}/linker_script.ld" CACHE INTERNAL "")
  ENDIF()

  TARGET_LINK_OPTIONS(${target}  PRIVATE "-T${SCRIPT}")
  message(STATUS "Adding ESPRESSIF libaries...")
  target_link_libraries(${target} PRIVATE idf::freertos idf::esp_common)
  idf_build_executable(${target}.elf)

ENDMACRO()
MACRO(HASH_GD32_FIRMWARE target)
ENDMACRO()

MACRO(USE_LIBRARY lib)
  add_subdirectory(${AF_FOLDER}/libraries/${lib})
  include_directories(${AF_FOLDER}/libraries/${lib})
  LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
