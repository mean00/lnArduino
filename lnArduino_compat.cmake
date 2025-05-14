

ADD_LIBRARY(gd32Arduino  STATIC  arduinoLayer/src/wrapper.cpp arduinoLayer/src/Wire.cpp ${LN_MCU_FOLDER}/boards/${GD32_BOARD}/lnPinMapping.cpp)
target_include_directories(gd32Arduino PRIVATE arduinoLayer/include include FreeRTOS/include . ${LN_MCU_FOLDER}/boards private_include ${LN_MCU_FOLDER}/include)
target_link_libraries( gd32Arduino lnArduinoInternal)
