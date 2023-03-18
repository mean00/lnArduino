include(../setup.cmake)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../private_include)
IF("${LN_MCU}" STREQUAL "CH32V3x")
    SET(EXTRA_OVERLAY ../riscv_ch32v3x/lnRCU_ch32v3x.cpp)
 ENDIF()

 SET(TARGET_SPECIFIC_SOURCES
                       ../${LN_EXT}/lnRCU_systemClock_${LN_EXT}.cpp 
                       ../${LN_EXT}/lnIRQ_${LN_EXT}.cpp 
                       ../${LN_EXT}/lnRunTime_${LN_EXT}.cpp
                       ../${LN_EXT}/lnSystemTimer_${LN_EXT}.cpp 
                       ../${LN_EXT}/lnCpuID_${LN_EXT}.cpp 
                       )

SET(OVERLAY_SRCS       
                       ${TARGET_SPECIFIC_SOURCES}
                       MapleFreeRTOS1000_pp.cpp systemAlloc.cpp lnDebug.cpp lnSerial.cpp lnGPIO.cpp                      
                       lnDma.cpp 
                       lnSystemInit.cpp lnRCU.cpp 
                       lnExti.cpp
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
                       ${EXTRA_OVERLAY}
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
  IF("${LN_MCU}" STREQUAL "CH32V3x")
    SET(IMPL ../riscv_ch32v3x/lnUSBD_ch32v3x.cpp)    
  ELSE()    
    SET(IMPL lnUSBD.cpp)
  ENDIF()
  ADD_LIBRARY(gd32_usb_usbd STATIC  ${IMPL})                                    
  SET(TUSB ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/tinyUsb/src)
  target_include_directories( gd32_usb_usbd  PRIVATE ${TUSB}/common ${TUSB} ${TUSB}/..)
ENDIF(LN_ENABLE_USBD)


