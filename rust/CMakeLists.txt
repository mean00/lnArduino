
SET(C ${CMAKE_CURRENT_SOURCE_DIR}/rnArduino/c_interface)

include_directories( ${C} )

IF(LN_ENABLE_USBD)
    include(${AF_FOLDER}/libraries/tinyUsb/tiny.cmake)
    SET(USB ${C}/lnCDC_c.cpp
            ${C}/lnUSB_c.cpp
            )
ENDIF()

add_library(lnarduino_c_bindings STATIC 
                                        ${C}/lnSPI_c.cpp 
                                        ${C}/lnI2C_c.cpp 
                                        ${C}/lnFast_EventGroup_c.cpp
                                        ${USB} )
target_link_libraries(lnarduino_c_bindings lnArduinoInternal)

IF(LN_ENABLE_USBD)
    target_link_libraries(lnarduino_c_bindings tinyUsb)
ENDIF()


