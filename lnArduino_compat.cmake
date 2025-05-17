
ADD_LIBRARY(gd32Arduino  STATIC  ${LNARDUINO_ROOT}/arduinoLayer/src/wrapper.cpp ${LNARDUINO_ROOT}/arduinoLayer/src/Wire.cpp ${LN_MCU_FOLDER}/boards/${LN_BOARD_NAME}/lnPinMapping.cpp)
target_include_directories(gd32Arduino PRIVATE ${LNARDUINO_ROOT}/arduinoLayer/include ${LNARDUINO_ROOT}/include ${LNARDUINO_ROOT}/FreeRTOS/include . ${LN_MCU_FOLDER}/boards private_include ${LN_MCU_FOLDER}/include)
target_link_libraries( gd32Arduino lnArduinoInternal)
