SET(RP2040_FOLDER       ${LN_MCU_FOLDER}/tinyUsb/)
SET(PICO_USB_FOLDER     ${PICO_SDK_PATH}/lib/tinyusb/src/portable/raspberrypi/rp2040/)

SET(DRIVERS             ${RP2040_FOLDER}/rp2040_usb.cpp  ${RP2040_FOLDER}/lnUSBD_rp2040.cpp   ${LNSRC}/lnUsbStack.cpp ${LNSRC}/lnUsbStubs.cpp  ${PICO_USB_FOLDER}/dcd_rp2040.c ${PICO_USB_FOLDER}/rp2040_usb.c)
SET(LN_OPT_TUSB_MCU     OPT_MCU_RP2040)
SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)  

FOREACH( subdir 
                    src/host/hardware_uart/include/hardware/
                    src/rp2_common/hardware_uart/include/ 
                    src/rp2_common/hardware_gpio/include/ 
                    src/common/pico_time/include/
                    src/rp2_common/pico_stdio/include/ 
                    src/rp2_common/pico_platform/include/ 
                    / 
                    src/common/pico_base/include/ 
                    src/rp2_common/hardware_timer/include/ 
                    src/rp2_common/hardware_resets/include/ 
                    src/rp2_common/hardware_irq/include/ 
                    src/common/pico_stdlib/include/pico/
                    src/boards/include/boards/  
                    src/rp2040/hardware_structs/include/ 
                    src/rp2_common/hardware_base/include/ 
                    src/rp2040/hardware_regs/include
        )
        LIST(APPEND idir  ${PICO_SDK_PATH}/${subdir})

ENDFOREACH()        

SET(LN_TUSB_EXTRA_IDIR ${idir} )
#SET(LN_TUSB_EXTRA_DEF   "__force_inline=__always_inline")