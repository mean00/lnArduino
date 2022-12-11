include(${AF_FOLDER}/setup.cmake)
SET(C ${CMAKE_CURRENT_SOURCE_DIR}/rnArduino/c_interface)

include_directories( ${C} )

add_library(lnarduino_c_bindings STATIC ${C}/lnI2C_c.cpp)
