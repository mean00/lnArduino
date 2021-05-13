

#foreach(src Common.cpp   wiring.c)#   wiring_analog.c  wiring_digital.c) 
 #   LIST(APPEND GD32_ARD legacy/src/${src} )
#endforeach()


ADD_LIBRARY(gd32Arduino  STATIC  arduinoLayer/wrapper.cpp)
target_include_directories(gd32Arduino PRIVATE arduinoLayer include FreeRTOS/include . freeRTOS_extension/N200)
