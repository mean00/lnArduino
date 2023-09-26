
# CFG_TUSB_OS == OPT_OS_FREERTOS

SET(TUSB ${CMAKE_CURRENT_SOURCE_DIR}/src/src)
SET(LNSRC ${CMAKE_CURRENT_SOURCE_DIR}/lnSrc)



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


SET(CH32V3x_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/ch32v3x/)
SET(RP2040_FOLDER  ${CMAKE_CURRENT_SOURCE_DIR}/rp2040/)
IF(USE_CH32V3x)
    SET(LN_OPT_TUSB_MCU     OPT_MCU_CH32V307)
    INCLUDE_DIRECTORIES(    ${CH32V3x_FOLDER}  )
    IF( USE_CH32v3x_USB_HS ) # ${LN_MCU} STREQUAL "CH32V3x") 
        SET(DRIVERS             ${CH32V3x_FOLDER}/dcd_usbhs.cpp       ${LNSRC}/lnUsbStack.cpp  ${LNSRC}/lnUsbStubs.cpp)
        SET(LN_OPT_TUSB_MCU     OPT_MCU_CH32V307)
        IF(1) # No need for high speed at the moment
            SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)              
        ELSE()
            SET(LN_OPT_MODE         OPT_MODE_HIGH_SPEED)      
            ADD_DEFINITIONS(-DCFG_TUD_MAX_SPEED=OPT_MODE_HIGH_SPEED -DLN_USB_SPEED=TUSB_SPEED_HIGH -DTUD_OPT_HIGH_SPEED=1)        
        ENDIF()
    ELSE() # NOT highspeed
        SET(DRIVERS             ${CH32V3x_FOLDER}/dcd_usbfs.c  ${CH32V3x_FOLDER}/dcd_usbfs_platform.cpp     ${LNSRC}/lnUsbStack.cpp  ${LNSRC}/lnUsbStubs.cpp)
        IF(1) # No need for high speed at the moment
            SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)              
        ELSE()
            SET(LN_OPT_MODE         OPT_MODE_HIGH_SPEED)      
            ADD_DEFINITIONS(-DCFG_TUD_MAX_SPEED=OPT_MODE_HIGH_SPEED -DLN_USB_SPEED=TUSB_SPEED_HIGH -DTUD_OPT_HIGH_SPEED=1)        
        ENDIF()
     ENDIF()
ELSEIF(USE_RP2040)
            include_directories( /pico/pico-sdk/src/boards/include/boards/ /pico/pico-sdk/src/rp2040/hardware_structs/include/ /pico/pico-sdk/src/rp2_common/hardware_base/include/
            /pico/pico-sdk/src/rp2040/hardware_regs/include )
            #include_directories( /pico/pico-sdk/src/common/pico_base/include/)
            #include_directories( /pico/pico-sdk/)
            #include_directories( /pico/pico-sdk/src/rp2_common/pico_platform/include/)
            SET(TINY_RP_SRC_FOLDER  ${CMAKE_CURRENT_SOURCE_DIR}/src/src/portable/raspberrypi/rp2040/)
            SET(DRIVERS             ${RP2040_FOLDER}/rp2040_usb.cpp  ${TINY_RP_SRC_FOLDER}/rp2040_usb.c ${TINY_RP_SRC_FOLDER}/dcd_rp2040.c )
            SET(LN_OPT_TUSB_MCU     OPT_MCU_RP2040)
            SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)  
ELSE()                    
            SET(DRIVERS             ${LNSRC}/lnTinyUsbImpl.cpp        ${LNSRC}/lnUsbStack.cpp ${LNSRC}/lnUsbLL.cpp)
            SET(LN_OPT_TUSB_MCU     OPT_MCU_STM32F1)
            SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)  
            
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
    ADD_DEFINITIONS("-DCFG_TUSB_DEBUG=2")
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
target_include_directories(tinyUsb PRIVATE ${AF_FOLDER}/private_include)
target_include_directories(tinyUsb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(tinyUsb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(tinyUsb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private_include)
target_include_directories(tinyUsb PUBLIC ${TUSB})
target_include_directories(tinyUsb PUBLIC ${TUSB}/common)
target_include_directories(tinyUsb PUBLIC ${TUSB}/device)
target_include_directories(tinyUsb PUBLIC ${TUSB}/class/device/cdc)
target_include_directories(tinyUsb PUBLIC ${TUSB}/class/device/dfu)

IF("${LN_OPT_TUSB_MCU}" STREQUAL    "OPT_MCU_STM32F1")
        target_compile_definitions(tinyUsb PUBLIC STM32F103xB )
ENDIF()

target_link_libraries(tinyUsb lnArduinoInternal)
