
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/lnArduino/)

include(../common_mcuSelect.cmake)
SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/lnArduino.cmake)

        
