

SET(LONGDUINO_ARDUINO ${ARDUINO_GD32_FREERTOS}/Longduino_cmake/cores/arduino/)
SET(ARDUINO_VARIANT    ${ARDUINO_GD32_FREERTOS}/Longduino_cmake/variants/sipeed-longan-nano CACHE INTERNAL "")

foreach(src Common.cpp     main.cpp          Print.cpp   WInterrupts.c    wiring.c          wiring_pulse.c  WMath.cpp
IPAddress.cpp  PluggableUSB.cpp  Stream.cpp  wiring_analog.c  wiring_digital.c  wiring_shift.c  WString.cpp)
    LIST(APPEND GD32_ARD ${LONGDUINO_ARDUINO}/${src})
endforeach()


ADD_LIBRARY(gd32Arduino  STATIC  ${GD32_ARD} ${ARDUINO_VARIANT}/pins_arduino.c)
target_include_directories(gd32Arduino PRIVATE ${LONGDUINO_ARDUINO} ${ARDUINO_VARIANT})
