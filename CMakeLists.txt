
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.13)
SET(ARDUINO_GD32_FREERTOS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
MESSAGE(STATUS "ARDUINO_GD32_FREERTOS is ${ARDUINO_GD32_FREERTOS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${AF_FOLDER}/cmake CACHE INTERNAL "")
include(applyPatch)

APPLY_PATCH_IF_NEEDED(patch_tinyusb tinyusb_no_static.patch        libraries/tinyUsb      "dont use freertos static constructs ")

OPTION(LN_ENABLE_USBD "Enable USBD support")
OPTION(LN_ENABLE_I2C  "Enable I2C support" ON)
OPTION(LN_ENABLE_SPI  "Enable SPI support" ON)

OPTION(LN_ENABLE_RUST "Enable rust support" OFF)

IF(LN_BOOTLOADER_SIZE)
        SET(LN_BOOTLOADER_SIZE ${LN_BOOTLOADER_SIZE} CACHE INTERNAL "")
ELSE(LN_BOOTLOADER_SIZE)
        MESSAGE(STATUS "No bootloader reserved space.")
        SET(LN_BOOTLOADER_SIZE 0 CACHE INTERNAL "")
ENDIF(LN_BOOTLOADER_SIZE)


include(./freeRTOS.cmake)
include(./arduino.cmake)
include(./embeddedPrintf.cmake)

add_subdirectory(src)



IF(LN_ENABLE_RUST)
    add_subdirectory(rust)
ENDIF(LN_ENABLE_RUST)
