SET(CH32V3x_FOLDER ${LN_MCU_FOLDER}/tinyUsb/)
SET(TINY_FOLDER    ${AF_FOLDER}/libraries/tinyUsb/src/src/portable/wch)
#SET(LN_OPT_MODE         OPT_MODE_HIGH_SPEED)
#ADD_DEFINITIONS(-DCFG_TUD_MAX_SPEED=OPT_MODE_HIGH_SPEED -DLN_USB_SPEED=TUSB_SPEED_HIGH -DTUD_OPT_HIGH_SPEED=1)        
SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)
SET(LN_OPT_TUSB_MCU     OPT_MCU_CH32V307)
INCLUDE_DIRECTORIES(    ${CH32V3x_FOLDER}  )

IF( USE_CH32v3x_USB_HS ) 
    SET(DRIVERS              
                    ${LNSRC}/lnUsbStack.cpp
                    ${TINY_FOLDER}/dcd_ch32_usbhs.c
                    ${CH32V3x_FOLDER}/lnUsbStubs.cpp
                    ${CH32V3x_FOLDER}/dcd_usbhs_platform.cpp
            )
ELSE( )
    SET(DRIVERS     ${LNSRC}/lnUsbStack.cpp
                    #${CH32V3x_FOLDER}/dcd_usbfs.c  
                    ${TINY_FOLDER}/dcd_ch32_usbfs.c
                    ${CH32V3x_FOLDER}/dcd_usbfs_platform.cpp
                    ${CH32V3x_FOLDER}/lnUsbStubs.cpp
            )
ENDIF()        

