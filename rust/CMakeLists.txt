
SET(C ${CMAKE_CURRENT_SOURCE_DIR}/rnArduino/c_interface)

include_directories( ${C} )

IF(LN_ENABLE_USBD)
    include(${AF_FOLDER}/libraries/tinyUsb/tiny.cmake)
    SET(EXTRA ${EXTRA} ${C}/lnCDC_c.cpp ${C}/lnUSB_c.cpp)
    MESSAGE(STATUS "********************************************************************* LN_ENABLE_USBD ${LN_ENABLE_USBD}")
ENDIF()

IF(LN_ENABLE_SPI)
  SET(EXTRA ${EXTRA} ${C}/lnSPI_c.cpp)
ENDIF()

IF(LN_ENABLE_I2C)
  SET(EXTRA ${EXTRA} ${C}/lnI2C_c.cpp)
ENDIF()

add_library(lnarduino_c_bindings STATIC 
                                        ${EXTRA}
                                        ${C}/lnFast_EventGroup_c.cpp
                                        ${C}/lnTiming_adc_c.cpp
                                         )
target_link_libraries(lnarduino_c_bindings lnArduinoInternal)

IF(LN_ENABLE_USBD)
    target_link_libraries(lnarduino_c_bindings tinyUsb)
ENDIF()


