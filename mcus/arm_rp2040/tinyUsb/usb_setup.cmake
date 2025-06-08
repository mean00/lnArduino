SET(RP_CHIP rp2040)
SET(RP_CHIP2 RP2040)
ADD_DEFINITIONS("-DPICO_RP2040=1")
include(${ESPRIT_ROOT}/mcus/rp_common/tinyUsb/rp_usb_setup.cmake)


