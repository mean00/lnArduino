
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ArduinoToolchainGd32.cmake)
cmake_minimum_required(VERSION 3.20)
OPTION(LN_EXTERNAL_FREERTOS "Deactivate internal freertos (esp)")
OPTION(LN_EXTERNAL_MALLOC "Deactivate internal malloc (esp)")
IF(True) #NOT ESPRIT_ROOT)
  IF(NOT ESPRIT_ROOT)
    SET(ESPRIT_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
  ENDIF()

  MESSAGE(STATUS "ESPRIT_ROOT is ${ESPRIT_ROOT}")

  LIST(APPEND CMAKE_MODULE_PATH  ${ESPRIT_ROOT} ${ESPRIT_ROOT}/cmake)


  # Already merged APPLY_PATCH_IF_NEEDED(patch_tinyusb tinyusb_no_static.patch        libraries/tinyUsb      "dont use freertos static constructs ")

  OPTION(LN_ENABLE_USBD "Enable USBD support" OFF)
  OPTION(LN_ENABLE_I2C  "Enable I2C support" ON)
  OPTION(LN_ENABLE_SPI  "Enable SPI support" ON)
  OPTION(LN_ENABLE_UART "Enable Full uart support" OFF)
  OPTION(LN_ENABLE_TIMING_ADC "Enable ADC driver by a timer " ON)

  OPTION(LN_ENABLE_RUST "Enable rust support" OFF)

  # WCH32Vxx specific
  OPTION(USE_CH32v3x_HW_IRQ_STACK "Enable WCH HW interrupt stack push pop (CH32V3xx only)" TRUE)
  OPTION(USE_CH32v3x_FAST_IRQ  "Use the WCH fast IRQ mode (CH32V30x only)" TRUE)
  OPTION(USE_CH32v3x_USB_HS  "Use the USB HS interface (CH32V307 only)" FALSE)
  OPTION(USE_CH32v3x_USB_OTG "Use the USB OTG interface (CH32V30x only)" FALSE)

  # RP2xxx specific
  OPTION(USE_RP2350_PURE_RAM "Load and run from RAM (RP2350 only)" FALSE)
  OPTION(USE_RP2040_PURE_RAM "Load and run from RAM (RP2040 only)" FALSE)
  OPTION(USE_CH32v3x_PURE_RAM "Load and run from RAM (CH32vxx only)" FALSE)

  SET(LN_MCU_FOLDER ${ESPRIT_ROOT}/mcus/${LN_EXT}/ CACHE INTERNAL "")
  #
  ADD_LIBRARY(esprit_dev INTERFACE)
  target_include_directories(esprit_dev INTERFACE ${LN_MCU_FOLDER}/boards/${LN_BOARD_NAME}/) # needed to get the freertos_tuning.h
  target_include_directories(esprit_dev INTERFACE ${LN_MCU_FOLDER}/include/) # needed to get the freertos_tuning.h
  target_include_directories(esprit_dev INTERFACE ${ESPRIT_ROOT}/)
  target_include_directories(esprit_dev INTERFACE ${ESPRIT_ROOT}/include)
  target_include_directories(esprit_dev INTERFACE ${CMAKE_BINARY_DIR}) # To catch generated files
  target_include_directories(esprit_dev INTERFACE ${LN_MCU_FOLDER})
  target_include_directories(esprit_dev INTERFACE ${LN_MCU_FOLDER}/include)
  target_include_directories(esprit_dev INTERFACE esprit_dev)
  target_include_directories(esprit_dev INTERFACE embeddedPrintf    )



  IF(LN_BOOTLOADER_SIZE)
    SET(LN_BOOTLOADER_SIZE ${LN_BOOTLOADER_SIZE} CACHE INTERNAL "")
  ELSE(LN_BOOTLOADER_SIZE)
    MESSAGE(STATUS "No bootloader reserved space.")
    SET(LN_BOOTLOADER_SIZE 0 CACHE INTERNAL "")
  ENDIF(LN_BOOTLOADER_SIZE)

  IF(LN_EXTERNAL_FREERTOS)
    target_compile_definitions(esprit_dev INTERFACE LN_CUSTOM_FREERTOS)
  ELSE()
    include(lnFreeRTOS)
    target_include_directories(esprit_dev  INTERFACE FreeRTOS )
  ENDIF()
  include(lnEmbeddedPrint)

  add_subdirectory(${LN_MCU_FOLDER}/src)
  add_subdirectory(${ESPRIT_ROOT}/src)

  IF(LN_ENABLE_RUST)
    add_subdirectory(${ESPRIT_ROOT}/rust) # Generate C++->C for easy Rust binding
  ENDIF(LN_ENABLE_RUST)


  # a firmmware should link against that one


  #____________________________________________
  # Merge things into a single lib and single interface
  #____________________________________________
  set(merged_name "esprit_single_lib")
  set(libs_to_merge "${USED_LIBS}")
  set(libs_to_keep esprit_lib)

  target_link_libraries(esprit_dev INTERFACE lnBoard ln_utils embeddedPrintf )
  list(APPEND libs_to_merge ln_utils embeddedPrintf esprit_lib )
  IF(NOT LN_EXTERNAL_FREERTOS)
    list(APPEND libs_to_merge FreeRTOS )
  ENDIF()
  target_include_directories(esprit_dev INTERFACE ${ESPRIT_ROOT})
  target_include_directories(esprit_dev INTERFACE ${ESPRIT_ROOT}/include)
  target_include_directories(esprit_dev INTERFACE ${LN_MCU_FOLDER})
  target_include_directories(esprit_dev INTERFACE ${ESPRIT_ROOT}/embedded_printf)

  IF(LN_ENABLE_RUST)
    target_include_directories(esprit_dev INTERFACE lnarduino_c_bindings)
    list(APPEND libs_to_merge lnarduino_c_bindings )
  ENDIF()
  target_include_directories(esprit_dev INTERFACE embeddedPrintf    )


  foreach(lib ${libs_to_merge})
    get_target_property(cludes ${lib} INTERFACE_INCLUDE_DIRECTORIES)
    if(NOT "${cludes}" STREQUAL "cludes-NOTFOUND")
      list(APPEND all_includes ${cludes})
    endif()


    list(APPEND objs $<TARGET_OBJECTS:${lib}>)
  endforeach()

  MESSAGE(STATUS "******merged********* ${all_includes}")
  add_library(${merged_name} STATIC ${objs})

  set_target_properties(esprit_dev PROPERTIES
        INTERFACE_LINK_LIBRARIES "${libs_to_keep}"
   )
  get_target_property(original_includes esprit_dev INTERFACE_INCLUDE_DIRECTORIES)
  MESSAGE(STATUS "****original*********** ${original_includes}")
  list(APPEND all_includes ${original_includes})
  #  list(APPEND all_includes "${ESPRIT_ROOT}/mcus/${LN_EXT}/boards/${LN_BOARD_NAME}/")
  #list(APPEND all_includes "${ESPRIT_ROOT}/mcus/${LN_EXT}/include")
  MESSAGE(STATUS "******final********* ${all_includes}")
  set_target_properties(esprit_dev PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${all_includes}"
   )





ENDIF()



