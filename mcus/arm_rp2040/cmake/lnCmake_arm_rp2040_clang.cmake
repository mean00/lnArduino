include(FindPython3)
MACRO(GENERATE_GD32_FIRMWARE target)

  #MESSAGE(STATUS " *** LN_MCU_FOLDER ${LN_MCU_FOLDER}")
    configure_file( "${LN_MCU_FOLDER}/boards/${GD32_BOARD}/rp2040_linker.ld.in" "${CMAKE_BINARY_DIR}/linker_script.ld" @ONLY)
    ADD_EXECUTABLE(${target} ${ARGN}  ${LN_MCU_FOLDER}/start.S   ${LN_MCU_FOLDER}/start.cpp      ${LN_MCU_FOLDER}/vector_table.S) 
    TARGET_LINK_LIBRARIES(${target} pico_stdlib)
    TARGET_LINK_LIBRARIES(${target} ${USED_LIBS} lnArduino) # duplicates are NOT a mistake !
    # duplicates are NOT a mistake !
    # TARGET_LINK_LIBRARIES(${target} embeddedPrintf gd32_overlay gd32Arduino   FreeRTOS  gd32_lowlevel c ) 
    TARGET_LINK_LIBRARIES(${target} embeddedPrintf  FreeRTOS   c ) 
    IF(LN_CUSTOM_LD_SCRIPT)
        SET(SCRIPT ${LN_CUSTOM_LD_SCRIPT} CACHE INTERNAL "")
    ELSE()
        SET(SCRIPT "${CMAKE_BINARY_DIR}/linker_script.ld" CACHE INTERNAL "")
    ENDIF()
    #TARGET_LINK_OPTIONS(${target}  PRIVATE "-T${SCRIPT}")
    pico_set_linker_script( ${target} ${LD_SCRIPT})
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

ENDMACRO(GENERATE_GD32_FIRMWARE target)

MACRO(USE_LIBRARY lib)
    add_subdirectory(${AF_FOLDER}/libraries/${lib})
    include_directories(${AF_FOLDER}/libraries/${lib})
    LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
