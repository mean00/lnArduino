# We ony enable usb for arm and non small footprint
IF(NOT DEFINED LN_ARCH)  
  SET(LN_ARCH            "ARM" CACHE INTERNAL "")
  SET(LN_MCU             "RP2040"  CACHE INTERNAL "")
  SET(LN_MCU_FLASH_SIZE  128   CACHE INTERNAL "")
  SET(LN_MCU_RAM_SIZE    64    CACHE INTERNAL "")
  SET(LN_MCU_SPEED       125000000       CACHE INTERNAL "")
  SET(LN_MCU_STATIC_RAM  10     CACHE INTERNAL "")
  SET(LN_MCU_EEPROM_SIZE 2     CACHE INTERNAL "")
  SET(LN_BOOTLOADER_SIZE 8     CACHE INTERNAL "")        
  SET(LN_SPEC            "picolibc"   CACHE INTERNAL "") # if not set we use nano        
  MESSAGE(STATUS "Architecture ${LN_ARCH}, MCU=${LN_MCU}")
ENDIF(NOT DEFINED LN_ARCH)

