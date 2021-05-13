include(../setup.cmake)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../private_include)
ADD_LIBRARY(gd32_overlay STATIC  MapleFreeRTOS1000_pp.cpp systemAlloc.cpp lnDebug.cpp lnSerial.cpp lnGPIO.cpp lnSPI.cpp lnSPI_dma.cpp lnDma.cpp lnIRQ.cpp lnSystemInit.cpp)
ADD_LIBRARY(gd32_lowlevel STATIC  systemHelper.cpp lnStubs.cpp)

        


