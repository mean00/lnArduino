include(FindPython3)
include(ln_merge_libs)
MACRO(GENERATE_GD32_FIRMWARE target)
  LN_MERGE_LIBS()
  configure_file( "${LN_MCU_FOLDER}/boards/bluepill/clang.lld.in" "${CMAKE_BINARY_DIR}/linker_script.ld" @ONLY)
  ADD_EXECUTABLE(${target} ${ARGN}  ${LN_MCU_FOLDER}/src/start.S   ${LN_MCU_FOLDER}/src/start.cpp      ${LN_MCU_FOLDER}/src/vector_table.S)
  #TARGET_LINK_LIBRARIES(${target} PUBLIC ${USED_LIBS} ) # duplicates are NOT a mistake !
  # duplicates are NOT a mistake !
  TARGET_LINK_LIBRARIES(${target}  PUBLIC esprit_single_lib  )
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
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.tmp
        COMMAND python3 ${AF_FOLDER}/script/lnCRC32_armChecksum.py  $<TARGET_FILE:${target}>.tmp $<TARGET_FILE:${target}>.ck_bin
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating checksumed bin file"
    )
ENDMACRO()

MACRO(USE_LIBRARY lib)
  add_subdirectory(${AF_FOLDER}/libraries/${lib})
  include_directories(${AF_FOLDER}/libraries/${lib})
  LIST(APPEND USED_LIBS ${lib})
  target_link_libraries(esprit_lib PUBLIC ${lib})
ENDMACRO(USE_LIBRARY lib)
