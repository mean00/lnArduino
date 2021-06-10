include( ${ARDUINO_GD32_FREERTOS}/freeRTOS_port.cmake)

include_directories(${ARDUINO_GD32_FREERTOS}/)
include_directories(${ARDUINO_GD32_FREERTOS}/include)
include_directories(${ARDUINO_GD32_FREERTOS}/freeRTOS_extension)
include_directories(${ARDUINO_GD32_FREERTOS}/FreeRTOS)
include_directories(${ARDUINO_GD32_FREERTOS}/FreeRTOS/include)
include_directories(${LN_FREERTOS_PORT})
include_directories(${ARDUINO_GD32_FREERTOS}/legacy/include/)
include_directories(${ARDUINO_GD32_FREERTOS}/legacy/GD32VF103_Firmware_Library/GD32VF103_standard_peripheral)
include_directories(${ARDUINO_GD32_FREERTOS}/legacy/GD32VF103_Firmware_Library/GD32VF103_standard_peripheral/Include)
include_directories(${ARDUINO_GD32_FREERTOS}/legacy/GD32VF103_Firmware_Library/RISCV/drivers)
include_directories(${ARDUINO_GD32_FREERTOS}/legacy/boards/${GD32_BOARD}/)
include_directories(${ARDUINO_GD32_FREERTOS}/arduinoLayer)


include( ${ARDUINO_GD32_FREERTOS}/cmake/lnCmake.cmake)
