
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
USE_LIBRARY(WS2812B)

SET(LIBSSD ${SSD1306}/ssd1306_base.cpp ${SSD1306}/ssd1306_ex.cpp ${SSD1306}/ssd1306_i2c_lnGd32vf103.cpp)

 
SET(ST7735_DRIVER ${AF_FOLDER}/libraries/simplerST7735/platformExample/longanNano/gd32ST7735.cpp)
include_directories(ST7735_DRIVER ${AF_FOLDER}/libraries/simplerST7735/platformExample/longanNano/)


USE_LIBRARY(simplerST7735)
       

 
GENERATE_GD32_FIRMWARE(everything hello.cpp ${LIBSSD} )       
        
