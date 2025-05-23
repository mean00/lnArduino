INCLUDE(FindPython3)
#
#
#
MACRO(RP_PIO_GENERATE sourcefile target)
  ADD_CUSTOM_COMMAND(OUTPUT ${target}
                   DEPENDS ${sourcefile}
                   COMMAND pioasm ${sourcefile} ${target}
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Compiling pio asm... "
    )

ENDMACRO()
#
#
#
MACRO(GENERATE_GD32_FIRMWARE target)
  #MESSAGE(STATUS " *** LN_MCU_FOLDER ${LN_MCU_FOLDER}")
  IF(USE_RP2350_PURE_RAM)
    CONFIGURE_FILE("${LN_MCU_FOLDER}/boards/${GD32_BOARD}/rp2350_linker_ram.ld.in" "${CMAKE_BINARY_DIR}/linker_script.ld" @ONLY)
  ELSE()
    CONFIGURE_FILE("${LN_MCU_FOLDER}/boards/${GD32_BOARD}/rp2350_linker.ld.in" "${CMAKE_BINARY_DIR}/linker_script.ld" @ONLY)
  ENDIF()
  ADD_EXECUTABLE(${target} ${ARGN})
  TARGET_LINK_LIBRARIES(${target} rplib)
  TARGET_LINK_LIBRARIES(${target} ${USED_LIBS} lnArduino) # duplicates are NOT a mistake !
  # duplicates are NOT a mistake !
  # TARGET_LINK_LIBRARIES(${target} embeddedPrintf gd32_overlay gd32Arduino   FreeRTOS  gd32_lowlevel c )
  TARGET_LINK_LIBRARIES(${target} rplib embeddedPrintf  FreeRTOS)
  IF(LN_CUSTOM_LD_SCRIPT)
    SET(SCRIPT ${LN_CUSTOM_LD_SCRIPT} CACHE INTERNAL "")
  ELSE()
    SET(SCRIPT "${CMAKE_BINARY_DIR}/linker_script.ld" CACHE INTERNAL "")
  ENDIF()
  TARGET_LINK_OPTIONS(${target}  PRIVATE "-T${SCRIPT}")
  #pico_set_linker_script( ${target} ${LD_SCRIPT})
  ADD_CUSTOM_COMMAND(TARGET ${target}
                   POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.bin
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Generating bin file"
    )
  ADD_CUSTOM_COMMAND(TARGET ${target}
                   POST_BUILD
                   COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${target}>
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Memory summary"
    )
  IF(USE_RP2350_PURE_RAM)
  ELSE()
    FIND_PROGRAM (picotool NAMES picotool)
    IF("x${picotool}" STREQUAL "xpicotool-NOTFOUND")
      MESSAGE(WARNING "picotool not found, it is part of the pico SDK")
    ELSE()
      ADD_CUSTOM_COMMAND(TARGET ${target}
            POST_BUILD
            COMMAND picotool uf2 convert  $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.uf2 --family rp2350-arm-s --abs-block
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating uf2 files"
            )
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(USE_LIBRARY lib)
  ADD_SUBDIRECTORY(${AF_FOLDER}/libraries/${lib})
  INCLUDE_DIRECTORIES(${AF_FOLDER}/libraries/${lib})
  LIST(APPEND USED_LIBS ${lib})
ENDMACRO()

MACRO(HASH_GD32_FIRMWARE target)
ENDMACRO()
