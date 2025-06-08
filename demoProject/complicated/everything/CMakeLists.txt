CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

INCLUDE(../common_CMakeLists.cmake)

PROJECT(Longan_SSD1306 C CXX ASM)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})        
ADD_SUBDIRECTORY(${ESPRIT_ROOT})


INCLUDE(${ESPRIT_ROOT}/setup.cmake)
        
SET( SSD1306 ${ESPRIT_ROOT}/libraries/simplerSSD1306 )
INCLUDE_DIRECTORIES( ${SSD1306} )
USE_LIBRARY(WS2812B)

SET(LIBSSD ${SSD1306}/ssd1306_base.cpp ${SSD1306}/ssd1306_ex.cpp ${SSD1306}/ssd1306_i2c_lnGd32vf103.cpp)

 
SET(ST7735_DRIVER ${ESPRIT_ROOT}/libraries/simplerST7735/platformExample/longanNano/gd32ST7735.cpp)
INCLUDE_DIRECTORIES(ST7735_DRIVER ${ESPRIT_ROOT}/libraries/simplerST7735/platformExample/longanNano/)


USE_LIBRARY(simplerST7735)
       

 
GENERATE_GD32_FIRMWARE(everything hello.cpp ${LIBSSD} )       
        
