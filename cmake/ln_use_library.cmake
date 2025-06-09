
MACRO(USE_LIBRARY lib)
  add_subdirectory(${ESPRIT_ROOT}/libraries/${lib})
  include_directories(${ESPRIT_ROOT}/libraries/${lib})
  LIST(APPEND USED_LIBS ${lib})
ENDMACRO(USE_LIBRARY lib)
