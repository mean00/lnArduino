

ADD_LIBRARY(gd32Arduino  STATIC  arduinoLayer/src/wrapper.cpp legacy/boards/${GD32_BOARD}/lnPinMapping.cpp)
target_include_directories(gd32Arduino PRIVATE arduinoLayer/include include FreeRTOS/include . freeRTOS_extension/N200 legacy/include private_include)
