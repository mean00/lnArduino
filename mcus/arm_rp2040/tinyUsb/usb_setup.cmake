SET(RP2040_FOLDER       ${LN_MCU_FOLDER}/tinyUsb/)
SET(PICO_USB_FOLDER     ${PICO_SDK_PATH}/lib/tinyusb/src/portable/raspberrypi/rp2040/)

SET(DRIVERS             ${RP2040_FOLDER}/rp2040_usb.cpp  ${RP2040_FOLDER}/lnUSBD_rp2040.cpp   ${LNSRC}/lnUsbStack.cpp ${LNSRC}/lnUsbStubs.cpp  ${PICO_USB_FOLDER}/dcd_rp2040.c ${PICO_USB_FOLDER}/rp2040_usb.c)
SET(LN_OPT_TUSB_MCU     OPT_MCU_RP2040)
SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)  

SET(LN_TUSB_EXTRA_IDIR  /pico/pico-sdk/src/host/hardware_uart/include/hardware/uart.h
/pico/pico-sdk/src/rp2_common/hardware_uart/include/ /pico/pico-sdk/src/rp2_common/hardware_gpio/include/ /pico/pico-sdk/src/common/pico_time/include/ /pico/pico-sdk/src/rp2_common/pico_stdio/include/ /pico/pico-sdk/src/rp2_common/pico_platform/include/ /pico/pico-sdk/ /pico/pico-sdk/src/common/pico_base/include/ /pico/pico-sdk/src/rp2_common/hardware_timer/include/ /pico/pico-sdk/src/rp2_common/hardware_resets/include/ /pico/pico-sdk/src/rp2_common/hardware_irq/include/ /pico/pico-sdk/src/common/pico_stdlib/include/pico/ ${PICO_SDK_PATH}/src/boards/include/boards/  ${PICO_SDK_PATH}/src/rp2040/hardware_structs/include/ ${PICO_SDK_PATH}/src/rp2_common/hardware_base/include/ ${PICO_SDK_PATH}/src/rp2040/hardware_regs/include)
SET(LN_TUSB_EXTRA_DEF   "__force_inline=__always_inline")