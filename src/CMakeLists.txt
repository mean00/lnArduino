include(../setup.cmake)
ADD_LIBRARY(gd32_overlay STATIC  MapleFreeRTOS1000_pp.cpp systemAlloc.cpp lnDebug.cpp)
ADD_LIBRARY(gd32_lowlevel STATIC  systemHelper.cpp)
        


