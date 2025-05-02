
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


  add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.bin
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating bin file"
    )
  add_custom_command(TARGET ${target}
                   POST_BUILD
                   COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${target}>
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Memory summary"
    )
ENDMACRO()
MACRO(HASH_GD32_FIRMWARE target)
  add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND python3 ${AF_FOLDER}/script/lnCRC32Checksum.py  $<TARGET_FILE:${target}>.bin $<TARGET_FILE:${target}>.ck_bin
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating checksumed file"
    )

ENDMACRO()

MACRO(USE_LIBRARY lib)
  add_subdirectory(${AF_FOLDER}/libraries/${lib})
  include_directories(${AF_FOLDER}/libraries/${lib})
  LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
