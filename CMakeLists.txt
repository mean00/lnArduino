
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.13)

IF(NOT ARDUINO_GD32_FREERTOS)

        SET(ARDUINO_GD32_FREERTOS ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ARDUINO_GD32_FREERTOS}/cmake CACHE INTERNAL "")

        MESSAGE(STATUS "ARDUINO_GD32_FREERTOS is ${ARDUINO_GD32_FREERTOS}")
        
        OPTION(LN_ENABLE_RUST "Enable rust support" OFF) 


        include(applyPatch)

        # Already merged APPLY_PATCH_IF_NEEDED(patch_tinyusb tinyusb_no_static.patch        libraries/tinyUsb      "dont use freertos static constructs ")

        OPTION(LN_ENABLE_USBD "Enable USBD support")
        OPTION(LN_ENABLE_I2C  "Enable I2C support" ON)
        OPTION(LN_ENABLE_SPI  "Enable SPI support" ON)

        OPTION(LN_ENABLE_RUST "Enable rust support" OFF)

        OPTION(USE_CH32v3x_USB_HS  "Use the USB HS interface (CH32V307 only)" FALSE)
        OPTION(USE_CH32v3x_USB_OTG "Use the USB OTG interface (CH32V30x only)" FALSE)

        SET(LN_MCU_FOLDER ${ARDUINO_GD32_FREERTOS}/mcus/${LN_EXT}/ CACHE INTERNAL "")
        include( ${LN_MCU_FOLDER}/freeRTOS_port_${LN_EXT}.cmake)

        # Ok let's create our internal interface libary
        ADD_LIBRARY(lnArduinoInternal INTERFACE)
        target_include_directories(lnArduinoInternal INTERFACE ${LN_MCU_FOLDER}/boards/${GD32_BOARD}/)
        target_include_directories(lnArduinoInternal INTERFACE ${ARDUINO_GD32_FREERTOS}/${LN_EXT}/boards/${GD32_BOARD}/)
        target_include_directories(lnArduinoInternal INTERFACE ${ARDUINO_GD32_FREERTOS}/)
        target_include_directories(lnArduinoInternal INTERFACE ${ARDUINO_GD32_FREERTOS}/include)
        target_include_directories(lnArduinoInternal INTERFACE ${ARDUINO_GD32_FREERTOS}/FreeRTOS)
        target_include_directories(lnArduinoInternal INTERFACE ${ARDUINO_GD32_FREERTOS}/FreeRTOS/include)
        target_include_directories(lnArduinoInternal INTERFACE ${LN_FREERTOS_PORT})
        target_include_directories(lnArduinoInternal INTERFACE ${ARDUINO_GD32_FREERTOS}/${LN_EXT}/include/)
        target_include_directories(lnArduinoInternal INTERFACE ${ARDUINO_GD32_FREERTOS}/arduinoLayer/include)
        target_include_directories(lnArduinoInternal INTERFACE ${CMAKE_BINARY_DIR}) # To catch generated files


        IF(LN_BOOTLOADER_SIZE)
                SET(LN_BOOTLOADER_SIZE ${LN_BOOTLOADER_SIZE} CACHE INTERNAL "")
        ELSE(LN_BOOTLOADER_SIZE)
                MESSAGE(STATUS "No bootloader reserved space.")
                SET(LN_BOOTLOADER_SIZE 0 CACHE INTERNAL "")
        ENDIF(LN_BOOTLOADER_SIZE)


        include(./freeRTOS.cmake)
        include(./arduino.cmake)
        include(./embeddedPrintf.cmake)

        add_subdirectory(src)

        IF(LN_ENABLE_RUST)
                add_subdirectory(rust) # Generate C++->C for easy Rust binding
        ENDIF(LN_ENABLE_RUST)


        # Ok let's create our public libary
        # a firmmware should link against that one
        ADD_LIBRARY(lnArduino INTERFACE)
        target_link_libraries(lnArduino INTERFACE  gd32Arduino FreeRTOS embeddedPrintf lnArduinoInternal)
        target_include_directories(lnArduino INTERFACE ${ARDUINO_GD32_FREERTOS})
        IF(LN_ENABLE_USBD) 
          TARGET_LINK_LIBRARIES(lnArduino INTERFACE gd32_usb_usbd )       
        ENDIF(LN_ENABLE_USBD)  
ENDIF(NOT ARDUINO_GD32_FREERTOS)        