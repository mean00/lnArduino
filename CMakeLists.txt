
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.0)
SET(ARDUINO_GD32_FREERTOS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
MESSAGE(STATUS "ARDUINO_GD32_FREERTOS is ${ARDUINO_GD32_FREERTOS}")

OPTION(LN_ENABLE_USBD "Enable USBD support")

include(./freeRTOS.cmake)
include(./arduino.cmake)
include(./embeddedPrintf.cmake)
        
add_subdirectory(src)
