SET(RP_CHIP rp2350)
SET(RP_CHIP2 RP2350)
ADD_DEFINITIONS("-DPICO_RP2350=1")
include(${AF_FOLDER}/mcus/rp_common/tinyUsb/rp_usb_setup.cmake)
