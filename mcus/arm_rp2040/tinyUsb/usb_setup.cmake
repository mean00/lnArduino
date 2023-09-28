include_directories( ${PICO_SDK}/src/boards/include/boards/ /pico/pico-sdk/src/rp2040/hardware_structs/include/ /pico/pico-sdk/src/rp2_common/hardware_base/include/
            /pico/pico-sdk/src/rp2040/hardware_regs/include )
            #include_directories( /pico/pico-sdk/src/common/pico_base/include/)
            #include_directories( /pico/pico-sdk/)
            #include_directories( /pico/pico-sdk/src/rp2_common/pico_platform/include/)
            SET(TINY_RP_SRC_FOLDER  ${CMAKE_CURRENT_SOURCE_DIR}/src/src/portable/raspberrypi/rp2040/)
            SET(DRIVERS             ${RP2040_FOLDER}/rp2040_usb.cpp  ${TINY_RP_SRC_FOLDER}/rp2040_usb.c ${TINY_RP_SRC_FOLDER}/dcd_rp2040.c )
            SET(LN_OPT_TUSB_MCU     OPT_MCU_RP2040)
            SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)  

