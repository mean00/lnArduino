
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#hardcode RP2040 + Clang
SET(USE_CLANG           True CACHE INTERNAL "")    
SET(USE_RP2040          True CACHE INTERNAL "")
SET(USE_RP2040_PURE_RAM True CACHE INTERNAL "")
SET(PICO_SDK_PATH /pico/pico-sdk CACHE INTERNAL "") # For PICO2040

SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/lnArduino/)

include(../common_mcuSelect.cmake)
include(../common_platformConfig.cmake)
SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/lnArduino.cmake)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)


        
