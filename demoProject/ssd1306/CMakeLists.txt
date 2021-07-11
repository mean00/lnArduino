
SET(AF_FOLDER  ${CMAKE_SOURCE_DIR}/Arduino_gd32_freeRTOS/)
include(./mcuSelect.cmake)
SET(CMAKE_TOOLCHAIN_FILE ${AF_FOLDER}/lnArduino.cmake)

cmake_minimum_required(VERSION 3.0)

PROJECT(Longan_SSD1306 C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        
SET( SSD1306 ${AF_FOLDER}/libraries/simplerSSD1306 )
INCLUDE_DIRECTORIES( ${SSD1306} )

SET(LIBSSD ${SSD1306}/ssd1306_base.cpp ${SSD1306}/ssd1306_ex.cpp ${SSD1306}/ssd1306_i2c_lnGd32vf103.cpp)
        
GENERATE_GD32_FIRMWARE(ssd1306 hello.cpp ${LIBSSD} )       
        
