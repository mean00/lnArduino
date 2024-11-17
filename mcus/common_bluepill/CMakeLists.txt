#
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../private_include)

IF(LN_DISABLE_DEFAULT_INIT)

ELSE()
  SET(LN_SYSTEM_INIT lnSystemInit.cpp  )
ENDIF()

IF(LN_DISABLE_DEFAULT_DMA)

ELSE()
  SET(LN_SYSTEM_DMA lnDma.cpp  )
ENDIF()


SET(OVERLAY_SRCS       
                      ${LN_SYSTEM_INIT}
                      ${LN_SYSTEM_DMA}
                       lnSerialCore.cpp 
                       lnGPIO.cpp                      
                       
                       lnRCU.cpp 
                       lnExti.cpp
                       lnBasicTimer.cpp
                       lnBasicDelayTimer.cpp
                       lnDelayTimer.cpp
                       lnTimer.cpp
                       lnDmaTimer.cpp
                       #lnStopWatch.cpp
                       lnDAC.cpp
                       lnADC.cpp
                       lnADC_base.cpp
                       lnADC_timing.cpp
                       lnFMC.cpp
                       lnStubs.cpp
                       
                        lnSerialTxOnly.cpp
                        lnSerialTxOnlyDma.cpp
                        lnSerialTxOnlyDmaBuffer.cpp

#                       ${LN_MCU_FOLDER}/boards/${GD32_BOARD}/lnPinMapping.cpp
                       CACHE INTERNAL ""
                    )



ADD_LIBRARY(lnArduino_impl STATIC  ${OVERLAY_SRCS} )
TARGET_LINK_LIBRARIES(lnArduino_impl  lnArduinoInternal)
TARGET_INCLUDE_DIRECTORIES(lnArduino_impl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

IF(LN_ENABLE_UART)
  target_sources( lnArduino_impl PRIVATE lnSerialRxTx.cpp lnSerialRxTxDma.cpp  )
  target_compile_options(lnArduino_impl PRIVATE "-DLN_ENABLE_UART=1")
ENDIF()

MACRO(ALTER nm text)
  IF(${nm})
    MESSAGE(STATUS "Altering ${text} to ${${nm}}")
    TARGET_COMPILE_DEFINITIONS(lnArduino_impl PRIVATE ${nm}=${${nm}})
  ENDIF()
ENDMACRO()

ALTER(LN_INITIAL_STACK_SIZE "initial stack size")
ALTER(LN_INITIAL_TASK_PRIORITY "initial task priority")


TARGET_COMPILE_DEFINITIONS(lnArduino_impl PRIVATE LN_MCU_SPEED=${LN_MCU_SPEED})
TARGET_INCLUDE_DIRECTORIES(lnArduino_impl PRIVATE arduinoLayer/include include FreeRTOS/include . ${LN_MCU_FOLDER}/boards private_include ${LN_MCU_FOLDER}/include)
IF(LN_USE_INTERNAL_CLOCK)                                     
        TARGET_COMPILE_DEFINITIONS(lnArduino_impl PRIVATE LN_USE_INTERNAL_CLOCK)
ENDIF(LN_USE_INTERNAL_CLOCK)                                     


#


IF(LN_ENABLE_I2C)
  target_sources(lnArduino_impl PRIVATE lnI2C.cpp lnI2C_impl.cpp  )  
ENDIF(LN_ENABLE_I2C)

IF(LN_ENABLE_SPI)
  target_sources(lnArduino_impl PRIVATE lnSPI_bp.cpp lnSPI_bp_dma.cpp  )
ENDIF(LN_ENABLE_SPI)


#
#
#
