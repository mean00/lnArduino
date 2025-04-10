
SET(C ${CMAKE_CURRENT_SOURCE_DIR}/rnArduino/c_interface)

INCLUDE_DIRECTORIES( ${C} )

IF(LN_ENABLE_USBD)
  INCLUDE(${AF_FOLDER}/libraries/tinyUsb/tiny.cmake)
  SET(EXTRA ${EXTRA} ${C}/lnCDC_c.cpp ${C}/lnUSB_c.cpp)
ENDIF()

IF(LN_ENABLE_SPI)
  SET(EXTRA ${EXTRA} ${C}/lnSPI_c.cpp)
ENDIF()

IF(USE_RP2040 OR USE_RP2350)
ELSE()
  SET(EXTRA ${EXTRA} ${C}/lnTiming_adc_c.cpp)
  IF(LN_ENABLE_I2C)
    SET(EXTRA ${EXTRA} ${C}/lnI2C_c.cpp)
  ENDIF()
ENDIF()

ADD_LIBRARY(lnarduino_c_bindings STATIC
    ${EXTRA}
    ${C}/lnFast_EventGroup_c.cpp
  )
TARGET_LINK_LIBRARIES(lnarduino_c_bindings lnArduinoInternal)

IF(LN_ENABLE_USBD)
  TARGET_LINK_LIBRARIES(lnarduino_c_bindings tinyUsb)
ENDIF()


