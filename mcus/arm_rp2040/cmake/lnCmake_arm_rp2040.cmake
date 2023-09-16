
MACRO(GENERATE_GD32_FIRMWARE target)    
    configure_file( "${LN_MCU_FOLDER}/boards/${GD32_BOARD}/rp2040_linker.ld.in" "${CMAKE_BINARY_DIR}/linker_script.ld" @ONLY)
    ADD_EXECUTABLE(${target} ${ARGN} )   
    TARGET_LINK_LIBRARIES(${target} rplib) 
    TARGET_LINK_LIBRARIES(${target} pico_stdlib) 
    TARGET_LINK_LIBRARIES(${target} ${USED_LIBS} lnArduino) # duplicates are NOT a mistake !
    # duplicates are NOT a mistake !
    # TARGET_LINK_LIBRARIES(${target} embeddedPrintf gd32_overlay gd32Arduino   FreeRTOS  gd32_lowlevel c  gcc ) 
    TARGET_LINK_LIBRARIES(${target} embeddedPrintf    FreeRTOS   c  gcc ) 
    IF(LN_CUSTOM_LD_SCRIPT)
        SET(SCRIPT ${LN_CUSTOM_LD_SCRIPT} CACHE INTERNAL "")
    ELSE()
        SET(SCRIPT "${CMAKE_BINARY_DIR}/linker_script.ld" CACHE INTERNAL "")
    ENDIF()

    add_custom_command(TARGET ${target}
                   POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${target}> $<TARGET_FILE:${target}>.bin
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMENT "Generating bin file"
    )

ENDMACRO(GENERATE_GD32_FIRMWARE target)

MACRO(USE_LIBRARY lib)
    add_subdirectory(${AF_FOLDER}/libraries/${lib})
    include_directories(${AF_FOLDER}/libraries/${lib})
    LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
