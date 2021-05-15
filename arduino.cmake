

ADD_LIBRARY(gd32Arduino  STATIC  arduinoLayer/wrapper.cpp)
target_include_directories(gd32Arduino PRIVATE arduinoLayer include FreeRTOS/include . freeRTOS_extension/N200 legacy/include)
