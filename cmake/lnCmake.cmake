
MACRO(GENERATE_GD32_FIRMWARE target)
    ADD_EXECUTABLE(${target} ${AF_FOLDER}/entry.S ${AF_FOLDER}/start.S ${ARGN})
    TARGET_LINK_LIBRARIES(${target} ${USED_LIBS} ) # duplicates are NOT a mistake !
    TARGET_LINK_LIBRARIES(${target} gd32_overlay gd32Arduino   FreeRTOS  gd32_lowlevel c  c gcc ) # dupicates are NOT a mistake !
ENDMACRO(GENERATE_GD32_FIRMWARE target)

MACRO(USE_LIBRARY lib)
    add_subdirectory(${AF_FOLDER}/libraries/${lib})
    include_directories(${AF_FOLDER}/libraries/${lib})
    LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
