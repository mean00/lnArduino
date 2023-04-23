
# CFG_TUSB_OS == OPT_OS_FREERTOS

SET(TUSB ${CMAKE_CURRENT_SOURCE_DIR}/src/src)
SET(LNSRC ${CMAKE_CURRENT_SOURCE_DIR}/lnSrc)
include_directories( ${AF_FOLDER}/private_include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/private_include)
include_directories( ${TUSB})
include_directories( ${TUSB}/common)
include_directories( ${TUSB}/device)
include_directories( ${TUSB}/class/device/cdc)
include_directories( ${TUSB}/class/device/dfu)

IF(NOT DEFINED LN_USB_NB_CDC)
    MESSAGE(FATAL_ERROR "SET LN_USB_NB_CDC to the number of CDC interface you use")
ENDIF(NOT DEFINED LN_USB_NB_CDC)

IF(NOT DEFINED LN_USB_NB_HID)
    MESSAGE(FATAL_ERROR "SET LN_USB_NB_CDC to the number of CDC interface you use")
ENDIF(NOT DEFINED LN_USB_NB_HID)

IF( DEFINED LN_USB_DFU_RUNTIME)
    SET(LN_USB_DFU_RUNTIME ${LN_USB_DFU_RUNTIME} CACHE INTERNAL "")
ELSE()
    SET(LN_USB_DFU_RUNTIME 0 CACHE INTERNAL "")
ENDIF()

IF( DEFINED LN_USB_DFU)
    SET(LN_USB_DFU ${LN_USB_DFU} CACHE INTERNAL "")
ELSE()
    SET(LN_USB_DFU 0 CACHE INTERNAL "")
ENDIF()


SET(CH32V3x_FOLDER      ${CMAKE_CURRENT_SOURCE_DIR}/ch32v3x/)

IF( USE_CH32v3x_USB_HS ) # ${LN_MCU} STREQUAL "CH32V3x") 
    SET(DRIVERS             ${CH32V3x_FOLDER}/dcd_usbhs.cpp       ${LNSRC}/lnUsbStack.cpp  ${LNSRC}/lnUsbStubs.cpp)
    SET(LN_OPT_TUSB_MCU     OPT_MCU_CH32V307)
    INCLUDE_DIRECTORIES(    ${CH32V3x_FOLDER}  )
    IF(1) # No need for high speed at the moment
        SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)              
    ELSE()
        SET(LN_OPT_MODE         OPT_MODE_HIGH_SPEED)      
        ADD_DEFINITIONS(-DCFG_TUD_MAX_SPEED=OPT_MODE_HIGH_SPEED -DLN_USB_SPEED=TUSB_SPEED_HIGH -DTUD_OPT_HIGH_SPEED=1)        
    ENDIF()
ELSE()
    IF( USE_CH32v3x_USB_OTG ) # ${LN_MCU} STREQUAL "CH32V3x") 
            SET(DRIVERS             ${CH32V3x_FOLDER}/dcd_usbfs.c  ${CH32V3x_FOLDER}/dcd_usbfs_platform.cpp     ${LNSRC}/lnUsbStack.cpp  ${LNSRC}/lnUsbStubs.cpp)
        SET(LN_OPT_TUSB_MCU     OPT_MCU_CH32V307)
        INCLUDE_DIRECTORIES(    ${CH32V3x_FOLDER}  )
        IF(1) # No need for high speed at the moment
            SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)              
        ELSE()
            SET(LN_OPT_MODE         OPT_MODE_HIGH_SPEED)      
            ADD_DEFINITIONS(-DCFG_TUD_MAX_SPEED=OPT_MODE_HIGH_SPEED -DLN_USB_SPEED=TUSB_SPEED_HIGH -DTUD_OPT_HIGH_SPEED=1)        
        ENDIF()
    ELSE()
        SET(DRIVERS             ${LNSRC}/lnTinyUsbImpl.cpp          ${LNSRC}/lnUsbStack.cpp ${LNSRC}/lnUsbLL.cpp)
        SET(LN_OPT_TUSB_MCU     OPT_MCU_STM32F1)
        SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)  
    ENDIF()
ENDIF()





configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/tusb_config.h.in ${CMAKE_BINARY_DIR}/tusb_config.h @ONLY)


IF( LN_USB_NB_CDC GREATER 0 )
        SET(CLASS_FILES ${CLASS_FILES} ${TUSB}/class/cdc/cdc_device.c ${LNSRC}/lnUsbCDC.cpp )
ENDIF()        
IF( LN_USB_NB_HID GREATER 0 )
        #SET(CLASS_FILES ${CLASS_FILES} ${TUSB}/class/cdc/cdc_device.c
ENDIF()        
IF( LN_USB_DFU_RUNTIME GREATER 0 )
        SET(CLASS_FILES ${CLASS_FILES} ${TUSB}/class/dfu/dfu_rt_device.c ${LNSRC}/lnUsbDFUrt.cpp)
ENDIF()        
   
IF( LN_USB_DFU GREATER 0 )
        SET(CLASS_FILES ${CLASS_FILES} ${TUSB}/class/dfu/dfu_device.c )
ENDIF()     
IF(FALSE)
    ADD_DEFINITIONS("-DCFG_TUSB_DEBUG=3")
    ADD_DEFINITIONS("-DCFG_TUSB_DEBUG_PRINTF=Logger_C")
ENDIF()


SET(SRCS  ${CLASS_FILES} 
          ${TUSB}/tusb.c
          ${TUSB}/device/usbd_control.c
          ${TUSB}/device/usbd.c
          ${TUSB}/common/tusb_fifo.c
          ${DRIVERS}
          )
add_library(tinyUsb STATIC ${SRCS})
target_include_directories(tinyUsb PRIVATE ${CMAKE_BINARY_DIR})
