

add_library(Esprit_impl STATIC)
TARGET_SOURCES(Esprit_impl PRIVATE ln_esp_gpio.cpp ln_esp_misc.cpp ln_esp_sysinit.cpp)
TARGET_LINK_LIBRARIES(Esprit_impl PRIVATE EspritInternal idf::driver idf::nvs_flash)
