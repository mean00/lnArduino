

ADD_LIBRARY(gd32Arduino  STATIC  arduinoLayer/src/wrapper.cpp arduinoLayer/src/Wire.cpp ${LN_EXT}/boards/${GD32_BOARD}/lnPinMapping.cpp)
target_include_directories(gd32Arduino PRIVATE arduinoLayer/include include FreeRTOS/include . ${LN_FREERTOS_PORT} ${LN_EXT}/boards private_include)
