LIST(APPEND CMAKE_MODULE_PATH ${ARDUINO_GD32_FREERTOS}/cmake)

include( ${ARDUINO_GD32_FREERTOS}/${LN_EXT}/freeRTOS_port_${LN_EXT}.cmake)

include_directories(${ARDUINO_GD32_FREERTOS}/${LN_EXT}/boards/${GD32_BOARD}/)

include_directories(${ARDUINO_GD32_FREERTOS}/)
include_directories(${ARDUINO_GD32_FREERTOS}/include)

include_directories(${ARDUINO_GD32_FREERTOS}/FreeRTOS)
include_directories(${ARDUINO_GD32_FREERTOS}/FreeRTOS/include)
include_directories(${LN_FREERTOS_PORT})
include_directories(${ARDUINO_GD32_FREERTOS}/${LN_EXT}/include/)

include_directories(${ARDUINO_GD32_FREERTOS}/arduinoLayer/include)

include_directories(${CMAKE_BINARY_DIR}) # To catch generated files

include( ${ARDUINO_GD32_FREERTOS}/cmake/lnCmake.cmake)
