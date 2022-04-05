
MACRO(GENERATE_GD32_FIRMWARE target)

  
    configure_file( "${AF_FOLDER}/legacy/boards/bluepill/ld.lds.in" "${CMAKE_BINARY_DIR}/linker_script.ld" @ONLY)
    ADD_EXECUTABLE(${target} ${ARGN}  ${AF_FOLDER}/arm_gd32fx/start.S   ${AF_FOLDER}/arm_gd32fx/start.cpp 
                                                                 ${AF_FOLDER}/arm_gd32fx/vector_table.S) 
    TARGET_LINK_LIBRARIES(${target} ${USED_LIBS} ) # duplicates are NOT a mistake !
    # duplicates are NOT a mistake !
    TARGET_LINK_LIBRARIES(${target} embeddedPrintf gd32_overlay gd32Arduino   FreeRTOS  gd32_lowlevel c  c gcc ) 
    IF(LN_CUSTOM_LD_SCRIPT)
        SET(SCRIPT ${LN_CUSTOM_LD_SCRIPT} CACHE INTERNAL "")
    ELSE()
        SET(SCRIPT "${CMAKE_BINARY_DIR}/linker_script.ld" CACHE INTERNAL "")
    ENDIF()
    TARGET_LINK_OPTIONS(${target}  PRIVATE "-T${SCRIPT}")
ENDMACRO(GENERATE_GD32_FIRMWARE target)

MACRO(USE_LIBRARY lib)
    add_subdirectory(${AF_FOLDER}/libraries/${lib})
    include_directories(${AF_FOLDER}/libraries/${lib})
    LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
