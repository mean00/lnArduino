
# CFG_TUSB_OS == OPT_OS_FREERTOS

SET(TUSB ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories( ${TUSB})
include_directories( ${TUSB}/common)
include_directories( ${TUSB}/device)
include_directories( ${TUSB}/class/device/cdc)
include_directories( /home/fx/Arduino_gd32/tinyusb/hw/mcu/st/cmsis_device_f1/Include)
include_directories( /home/fx/Arduino_gd32/tinyusb/lib/CMSIS_5/CMSIS/Core/Include)

IF(FALSE)
    ADD_DEFINITIONS("-DCFG_TUSB_DEBUG=3")
    ADD_DEFINITIONS("-DCFG_TUSB_DEBUG_PRINTF=Logger_C")
ENDIF()
SET(SRCS  ${TUSB}/tusb.c 
          ${TUSB}/device/usbd_control.c 
          ${TUSB}/device/usbd.c 
          ${TUSB}/common/tusb_fifo.c
          ${TUSB}/class/cdc/cdc_device.c 
          ${TUSB}/../cdc/cdc_usb_descriptors.c   
          ${TUSB}/../basic_cdc.cpp
          ${TUSB}/../tinyusb2ln.cpp
          )
add_library(tinyUsb STATIC ${SRCS})

