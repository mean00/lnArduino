ADD_LIBRARY(ST7735 
            simplerST7735.cpp 
            simplerST7735_ex.cpp 
            simplerST7735_ex_ll.cpp 
            ${ST7735_DRIVER})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
