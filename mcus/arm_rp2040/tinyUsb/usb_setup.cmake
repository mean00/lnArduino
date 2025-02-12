SET(RP2040_FOLDER       ${LN_MCU_FOLDER}/tinyUsb/)
SET(PICO_USB_FOLDER     ${PICO_SDK_PATH}/lib/tinyusb/src/portable/raspberrypi/rp2040/)
SET(NATIVE_DRIVER       ${AF_FOLDER}/libraries/tinyUsb/src/src/portable/raspberrypi/rp2040)
SET(DRIVERS             ${LNSRC}/lnUsbStack.cpp ${RP2040_FOLDER}/rp2040_usb.cpp  ${RP2040_FOLDER}/lnUSBD_rp2040.cpp   ${NATIVE_DRIVER}/dcd_rp2040.c ${NATIVE_DRIVER}/rp2040_usb.c)
SET(LN_OPT_TUSB_MCU     OPT_MCU_RP2040)
SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)

ADD_DEFINITIONS("-DPICO_RP2040=1")
#ADD_DEFINITIONS("-D__force_inline=__attribute__((always_inline))")
#ADD_DEFINITIONS("-D__force_inline=__attribute__((always_inline))")

FOREACH( subdir hardware_uart hardware_sync hardware_gpio pico_stdio pico_platform hardware_timer hardware_resets hardware_irq hardware_base   )
  LIST(APPEND idir  ${PICO_SDK_PATH}/src/rp2_common/${subdir}/include)
ENDFOREACH()

FOREACH( subdir
                    /
                    src/common/pico_time/include/
                    src/common/pico_base/include/
                    src/common/pico_stdlib/include/pico/
                    src/rp2040/hardware_structs/include/
                    src/rp2040/hardware_regs/include
                    src/boards/include/boards/
        )
  LIST(APPEND idir  ${PICO_SDK_PATH}/${subdir})
ENDFOREACH()

SET(LN_TUSB_EXTRA_IDIR ${idir} )

#  ${LNSRC}/lnUsbStubs.cpp
