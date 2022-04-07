include(../setup.cmake)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../private_include)
SET(OVERLAY_SRCS       MapleFreeRTOS1000_pp.cpp systemAlloc.cpp lnDebug.cpp lnSerial.cpp lnGPIO.cpp                      
                       lnDma.cpp 
                       lnSystemInit.cpp lnRCU.cpp 
                       lnRCU_systemClock_${LN_EXT}.cpp 
                       lnIRQ_${LN_EXT}.cpp 
                       lnRunTime_${LN_EXT}.cpp
                       
                       lnSystemTimer_${LN_EXT}.cpp 
                       lnCpuID_${LN_EXT}.cpp 
                       lnDAC.cpp lnExti.cpp
                       lnBasicTimer.cpp
                       lnBasicDelayTimer.cpp
                       lnDelayTimer.cpp
                       lnTimer.cpp
                       lnDmaTimer.cpp
                       lnStopWatch.cpp
                       lnDAC.cpp
                       lnADC.cpp
                       lnADC_base.cpp
                       lnADC_timing.cpp
                       lnFMC.cpp
                       lnFreeRTOSDebug.cpp                       
                       ../legacy/boards/${GD32_BOARD}/lnPinMapping.cpp
                    )

IF(LN_ENABLE_I2C)
  LIST(APPEND OVERLAY_SRCS lnI2C.cpp lnI2C_impl.cpp  )
  LN_APPEND_FLAGS(lnIRQ_${LN_EXT}.cpp -DLN_ENABLE_I2C)
ENDIF(LN_ENABLE_I2C)

IF(LN_ENABLE_SPI)
  LIST(APPEND OVERLAY_SRCS lnSPI.cpp lnSPI_dma.cpp  )
  LN_APPEND_FLAGS(lnIRQ_${LN_EXT}.cpp -DLN_ENABLE_SPI)
ENDIF(LN_ENABLE_SPI)

ADD_LIBRARY(gd32_overlay STATIC  ${OVERLAY_SRCS} )
ADD_LIBRARY(gd32_lowlevel STATIC  systemHelper.cpp lnStubs.cpp)

TARGET_COMPILE_DEFINITIONS(gd32_overlay PRIVATE LN_MCU_SPEED=${LN_MCU_SPEED})
IF(LN_USE_INTERNAL_CLOCK)                                     
        TARGET_COMPILE_DEFINITIONS(gd32_overlay PRIVATE LN_USE_INTERNAL_CLOCK)
ENDIF(LN_USE_INTERNAL_CLOCK)                                     

If(LN_DEBUG_UART)
  MESSAGE(STATUS "Using uart ${LN_DEBUG_UART} for debug")
  TARGET_COMPILE_DEFINITIONS(gd32_overlay PRIVATE LN_DEBUG_UART=${LN_DEBUG_UART})
ENDIF()

#
#
#
IF(LN_ENABLE_USBD)
  ADD_LIBRARY(gd32_usb_usbd STATIC  lnUSBD.cpp)                                    
  SET(TUSB ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/tinyUsb/src)
  target_include_directories( gd32_usb_usbd  PRIVATE ${TUSB}/common ${TUSB} ${TUSB}/..)
ENDIF(LN_ENABLE_USBD)


