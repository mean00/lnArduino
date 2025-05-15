


add_library(ln_esp_lib STATIC)
TARGET_SOURCES(ln_esp_lib PRIVATE ln_esp_gpio.cpp)
TARGET_LINK_LIBRARIES(ln_esp_lib PRIVATE lnArduinoInternal idf::driver)
