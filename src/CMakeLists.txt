include(../setup.cmake)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../private_include)
ADD_LIBRARY(gd32_overlay STATIC  MapleFreeRTOS1000_pp.cpp systemAlloc.cpp lnDebug.cpp lnSerial.cpp lnGPIO.cpp 
                                 lnSPI.cpp lnSPI_dma.cpp 
                                 lnDma.cpp 
                                 lnSystemInit.cpp lnRCU.cpp 
                                 lnRCU_systemClock_${LN_EXT}.cpp 
                                 lnIRQ_${LN_EXT}.cpp 
                                 lnRunTime_${LN_EXT}.cpp
                                 lnI2C.cpp lnI2C_impl.cpp  
                                 lnSystemTimer_${LN_EXT}.cpp 
                                 lnDAC.cpp lnExti.cpp
                                 lnBasicTimer.cpp
                                 lnTimer.cpp
                                 lnStopWatch.cpp
                                 lnDAC.cpp
                                 lnADC.cpp
                                 ../legacy/boards/${GD32_BOARD}/lnPinMapping.cpp
                    )
ADD_LIBRARY(gd32_lowlevel STATIC  systemHelper.cpp lnStubs.cpp)

TARGET_COMPILE_DEFINITIONS(gd32_overlay PRIVATE LN_MCU_SPEED=${LN_MCU_SPEED})
                                     
                                         

        


