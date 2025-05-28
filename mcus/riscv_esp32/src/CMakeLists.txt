

add_library(lnArduino_impl STATIC)
TARGET_SOURCES(lnArduino_impl PRIVATE ln_esp_gpio.cpp ln_esp_misc.cpp ln_esp_sysinit.cpp)
TARGET_LINK_LIBRARIES(lnArduino_impl PRIVATE lnArduinoInternal idf::driver idf::nvs_flash)
