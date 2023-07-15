INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../private_include)
IF("${LN_MCU}" STREQUAL "CH32V3x")
    SET(EXTRA_OVERLAY ${LN_MCU_FOLDER}/lnRCU_ch32v3x.cpp)
 ENDIF()

include_directories( ${LN_MCU_FOLDER} )


 SET(TARGET_SPECIFIC_SOURCES
                       ${LN_MCU_FOLDER}/lnRCU_systemClock_${LN_EXT}.cpp 
                       ${LN_MCU_FOLDER}/lnIRQ_${LN_EXT}.cpp 
                       ${LN_MCU_FOLDER}/lnRunTime_${LN_EXT}.cpp
                       ${LN_MCU_FOLDER}/lnSystemTimer_${LN_EXT}.cpp 
                       ${LN_MCU_FOLDER}/lnCpuID_${LN_EXT}.cpp 
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
                       ${LN_MCU_FOLDER}/boards/${GD32_BOARD}/lnPinMapping.cpp
                    )

IF(LN_ENABLE_I2C)
  LIST(APPEND OVERLAY_SRCS lnI2C.cpp lnI2C_impl.cpp  )
  LN_APPEND_FLAGS( ${LN_MCU_FOLDER}/lnIRQ_${LN_EXT}.cpp -DLN_ENABLE_I2C)
ENDIF(LN_ENABLE_I2C)

IF(LN_ENABLE_SPI)
  LIST(APPEND OVERLAY_SRCS lnSPI.cpp lnSPI_dma.cpp  )
  LN_APPEND_FLAGS( ${LN_MCU_FOLDER}/lnIRQ_${LN_EXT}.cpp -DLN_ENABLE_SPI)
ENDIF(LN_ENABLE_SPI)

ADD_LIBRARY(gd32_overlay STATIC  ${OVERLAY_SRCS} )
TARGET_LINK_LIBRARIES(gd32_overlay  lnArduinoInternal)


MACRO(ALTER nm text)
  IF(${nm})
    MESSAGE(STATUS "Altering ${text} to ${${nm}}")
    TARGET_COMPILE_DEFINITIONS(gd32_overlay PRIVATE ${nm}=${${nm}})
  ENDIF()
ENDMACRO()

ALTER(LN_INITIAL_STACK_SIZE "initial stack size")
ALTER(LN_INITIAL_TASK_PRIORITY "initial task priority")




ADD_LIBRARY(gd32_lowlevel STATIC  systemHelper.cpp lnStubs.cpp)
TARGET_LINK_LIBRARIES(gd32_lowlevel  lnArduinoInternal)

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
  IF(USE_CH32v3x_USB_HS) #IF("${LN_MCU}" STREQUAL "CH32V3x")
    SET(IMPL ../riscv_ch32v3x/lnUSBD_ch32v3x.cpp)    
  ELSE()    
    SET(IMPL lnUSBD.cpp)
  ENDIF()
  ADD_LIBRARY(gd32_usb_usbd STATIC  ${IMPL})    
  TARGET_LINK_LIBRARIES(gd32_usb_usbd  lnArduinoInternal)                                
  SET(TUSB ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/tinyUsb/src)
  target_include_directories( gd32_usb_usbd  PRIVATE ${TUSB}/common ${TUSB} ${TUSB}/..)
ENDIF(LN_ENABLE_USBD)


