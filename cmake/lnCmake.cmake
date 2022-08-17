
include(lnCmake_${LN_EXT})

MACRO (LN_APPEND_FLAGS target)
	GET_SOURCE_FILE_PROPERTY(flags ${target} COMPILE_FLAGS)
	IF(flags  MATCHES "NOTFOUND")
			SET(flags "")
	ENDIF()
  SET(flags "${flags} ${ARGN}")
	SET_SOURCE_FILES_PROPERTIES(${target} PROPERTIES COMPILE_FLAGS "${flags}")   
ENDMACRO (LN_APPEND_FLAGS)



MACRO(USE_LIBRARY lib)
    add_subdirectory(${AF_FOLDER}/libraries/${lib})
    include_directories(${AF_FOLDER}/libraries/${lib})
    LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
