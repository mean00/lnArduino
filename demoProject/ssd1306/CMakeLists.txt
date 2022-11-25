cmake_minimum_required(VERSION 3.0)

include(../common_CMakeLists.cmake)

PROJECT(lnSSD1306 C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
       

SET( SSD1306 ${AF_FOLDER}/libraries/simplerSSD1306/platforms/lnArduino )
MESSAGE(STATUS "AF => <${AF_FOLDER}>")
MESSAGE(STATUS "SS => <${SSD1306}>")
INCLUDE_DIRECTORIES( ${SSD1306} )
INCLUDE_DIRECTORIES( ${SSD1306}/../.. )
add_subdirectory(${SSD1306} )
        
GENERATE_GD32_FIRMWARE(demo hello.cpp  )      
target_link_libraries(demo ssd1306)

