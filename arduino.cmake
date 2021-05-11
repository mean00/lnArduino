

foreach(src Common.cpp   wiring.c   wiring_analog.c  wiring_digital.c) 
    LIST(APPEND GD32_ARD legacy/src/${src} )
endforeach()


ADD_LIBRARY(gd32Arduino  STATIC  ${GD32_ARD} legacy/boards/${GD32_BOARD}/pins_arduino.c)
target_include_directories(gd32Arduino PRIVATE legacy/include legacy/boards/${GD32_BOARD})
