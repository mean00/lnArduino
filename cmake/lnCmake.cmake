
IF(USE_CLANG)
    SET(EXTRA "_clang")
ENDIF()    
include(${LN_MCU_FOLDER}/cmake/lnCmake_${LN_TOOLCHAIN_EXT}${EXTRA}.cmake)
#______________________________________

MACRO (LN_APPEND_FLAGS target)
    SET_PROPERTY(   SOURCE ${target} APPEND_STRING PROPERTY COMPILE_OPTIONS ${ARGN}) 
    GET_SOURCE_FILE_PROPERTY(flags ${target} COMPILE_FLAGS)
ENDMACRO (LN_APPEND_FLAGS)
#______________________________________
# This does not seem to work
#MACRO (LN_APPEND_IDIR target)
#GET_SOURCE_FILE_PROPERTY(flags ${target} INCLUDE_DIRECTORIES)
#MESSAGE(STATUS "aax ${target}=> ${flags} www")
#SET_PROPERTY(   
#SOURCE ${target}
#PROPERTY INCLUDE_DIRECTORIES ${ARGN}
#APPEND
#) 
#GET_SOURCE_FILE_PROPERTY(flags ${target} INCLUDE_DIRECTORIES)
#MESSAGE(STATUS "aat ${target}=>${flags} www")
#ENDMACRO (LN_APPEND_IDIR)


MACRO(USE_LIBRARY lib)
    add_subdirectory(${ARDUINO_GD32_FREERTOS}/libraries/${lib})
    include_directories(${ARDUINO_GD32_FREERTOS}/libraries/${lib})
    LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
