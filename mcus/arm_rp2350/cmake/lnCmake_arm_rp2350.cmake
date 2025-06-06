include(ln_merge_libs)

MACRO(GENERATE_GD32_FIRMWARE target)
  LN_MERGE_LIBS()
  CONFIGURE_FILE("${LN_MCU_FOLDER}/boards/${LN_BOARD_NAME}/rp2350_linker.ld.in" "${CMAKE_BINARY_DIR}/linker_script.ld" @ONLY)
  ADD_EXECUTABLE(${target} ${ARGN})
  TARGET_LINK_LIBRARIES(${target} PUBLIC esprit_dev esprit_single_lib) # duplicates are NOT a mistake !
  TARGET_LINK_LIBRARIES(${target} PUBLIC pico_stdlib)
  IF(LN_CUSTOM_LD_SCRIPT)
    SET(SCRIPT ${LN_CUSTOM_LD_SCRIPT} CACHE INTERNAL "")
  ELSE()
    SET(SCRIPT "${CMAKE_BINARY_DIR}/linker_script.ld" CACHE INTERNAL "")
  ENDIF()

  ADD_CUSTOM_COMMAND(TARGET ${target}
                   POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.bin
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Generating bin file"
    )
  FIND_PROGRAM(elf2uf2 NAMES elf2uf2)
  IF("x${elf2uf2}" STREQUAL "xelf2uf2-NOTFOUND")
    MESSAGE(WARNING "elf2uf2-rs not found, it is part of the pico SDK")
  ELSE()
    ADD_CUSTOM_COMMAND(TARGET ${target}
        POST_BUILD
        COMMAND elf2uf2  $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.uf2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating uf2 files"
        )
  ENDIF()
ENDMACRO()

include(ln_use_library)

MACRO(HASH_GD32_FIRMWARE target)
ENDMACRO()
