
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.20)
OPTION(LN_EXTERNAL_FREERTOS "Deactivate internal freertos (esp)")
OPTION(LN_EXTERNAL_MALLOC "Deactivate internal malloc (esp)")
IF(True) #NOT LNARDUINO_ROOT)
  IF(NOT LNARDUINO_ROOT)
    SET(LNARDUINO_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
  ENDIF()

  MESSAGE(STATUS "LNARDUINO_ROOT is ${LNARDUINO_ROOT}")

  LIST(APPEND CMAKE_MODULE_PATH  ${LNARDUINO_ROOT} ${LNARDUINO_ROOT}/cmake)


  # Already merged APPLY_PATCH_IF_NEEDED(patch_tinyusb tinyusb_no_static.patch        libraries/tinyUsb      "dont use freertos static constructs ")

  OPTION(LN_ENABLE_USBD "Enable USBD support" OFF)
  OPTION(LN_ENABLE_I2C  "Enable I2C support" ON)
  OPTION(LN_ENABLE_SPI  "Enable SPI support" ON)
  OPTION(LN_ENABLE_UART "Enable Full uart support" OFF)
  OPTION(LN_ENABLE_TIMING_ADC "Enable ADC driver by a timer " ON)

  OPTION(LN_ENABLE_RUST "Enable rust support" OFF)

  OPTION(USE_CH32v3x_HW_IRQ_STACK "Enable WCH HW interrupt stack push pop (CH32V3xx only)" TRUE)
  OPTION(USE_CH32v3x_FAST_IRQ  "Use the WCH fast IRQ mode (CH32V30x only)" TRUE)
  OPTION(USE_CH32v3x_USB_HS  "Use the USB HS interface (CH32V307 only)" FALSE)
  OPTION(USE_CH32v3x_USB_OTG "Use the USB OTG interface (CH32V30x only)" FALSE)

  OPTION(USE_RP2350_PURE_RAM "Load and run from RAM (RP2350 only)" FALSE)
  OPTION(USE_RP2040_PURE_RAM "Load and run from RAM (RP2040 only)" FALSE)
  OPTION(USE_CH32v3x_PURE_RAM "Load and run from RAM (CH32vxx only)" FALSE)

  SET(LN_MCU_FOLDER ${LNARDUINO_ROOT}/mcus/${LN_EXT}/ CACHE INTERNAL "")
  #
  ADD_LIBRARY(lnBoard INTERFACE)
  target_include_directories(lnBoard INTERFACE ${LN_MCU_FOLDER}/boards/${LN_BOARD_NAME}/) # needed to get the freertos_tuning.h
  target_include_directories(lnBoard INTERFACE ${LN_MCU_FOLDER}/include/) # needed to get the freertos_tuning.h

  # Ok let's create our internal interface libary
  ADD_LIBRARY(lnArduinoInternal INTERFACE)
  target_include_directories(lnArduinoInternal INTERFACE ${LNARDUINO_ROOT}/)
  target_include_directories(lnArduinoInternal INTERFACE ${LNARDUINO_ROOT}/include)
  target_include_directories(lnArduinoInternal INTERFACE ${LNARDUINO_ROOT}/arduinoLayer/include)
  target_include_directories(lnArduinoInternal INTERFACE ${CMAKE_BINARY_DIR}) # To catch generated files
  target_include_directories(lnArduinoInternal INTERFACE ${LN_MCU_FOLDER})
  target_include_directories(lnArduinoInternal INTERFACE ${LN_MCU_FOLDER}/include)
  target_include_directories(lnArduinoInternal INTERFACE lnBoard)



  IF(LN_BOOTLOADER_SIZE)
    SET(LN_BOOTLOADER_SIZE ${LN_BOOTLOADER_SIZE} CACHE INTERNAL "")
  ELSE(LN_BOOTLOADER_SIZE)
    MESSAGE(STATUS "No bootloader reserved space.")
    SET(LN_BOOTLOADER_SIZE 0 CACHE INTERNAL "")
  ENDIF(LN_BOOTLOADER_SIZE)

  IF(LN_EXTERNAL_FREERTOS)
    target_compile_definitions(lnArduinoInternal INTERFACE LN_CUSTOM_FREERTOS)
  ELSE()
    include(lnFreeRTOS)
    target_link_libraries(lnArduinoInternal  INTERFACE FreeRTOS )
  ENDIF()
  include(lnArduino_compat)
  include(lnEmbeddedPrint)

  add_subdirectory(${LN_MCU_FOLDER}/src)

  add_subdirectory(${LNARDUINO_ROOT}/src)

  IF(LN_ENABLE_RUST)
    add_subdirectory(${LNARDUINO_ROOT}/rust) # Generate C++->C for easy Rust binding
  ENDIF(LN_ENABLE_RUST)


  # Ok let's create our public libary
  # a firmmware should link against that one
  ADD_LIBRARY(lnArduino INTERFACE)
  target_link_libraries(lnArduino INTERFACE lnBoard ln_utils gd32Arduino embeddedPrintf lnArduinoInternal)
  IF(NOT LN_EXTERNAL_FREERTOS)
    target_link_libraries(lnArduino INTERFACE FreeRTOS )
  ENDIF()
  target_include_directories(lnArduino INTERFACE ${LNARDUINO_ROOT})
  target_include_directories(lnArduino INTERFACE ${LNARDUINO_ROOT}/include)
  target_include_directories(lnArduino INTERFACE ${LN_MCU_FOLDER})
ENDIF()
