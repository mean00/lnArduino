IF(USE_CH32v3x_HW_IRQ_STACK)
  ADD_DEFINITIONS("-DUSE_CH32v3x_HW_IRQ_STACK")
ENDIF()

SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)
SET(LN_OPT_TUSB_MCU     OPT_MCU_CH32V307)

IF(FALSE) # Old driver
  SET(CH32V3x_FOLDER ${LN_MCU_FOLDER}/tinyUsb/old)
  INCLUDE_DIRECTORIES(    ${CH32V3x_FOLDER}  )

  IF( USE_CH32v3x_USB_HS )
    MESSAGE(STATUS "Using old CH32 HS Driver")
    SET(DRIVERS
                        ${LNSRC}/lnUsbStack.cpp
                        ${CH32V3x_FOLDER}/dcd_ch32_usbhs.c
                        ${CH32V3x_FOLDER}/../lnUsbStubs.cpp
                        ${CH32V3x_FOLDER}/dcd_usbhs_platform.cpp
                )
  ELSE( )
    MESSAGE(STATUS "Using old CH32 OTG Driver")
    SET(DRIVERS     ${LNSRC}/lnUsbStack.cpp
                        #${CH32V3x_FOLDER}/dcd_usbfs.c
                        ${CH32V3x_FOLDER}/dcd_ch32_usbfs.c
                        ${CH32V3x_FOLDER}/dcd_usbfs_platform.cpp
                        ${CH32V3x_FOLDER}/../lnUsbStubs.cpp
                )
  ENDIF()

ELSE() # New driver

  IF( USE_CH32v3x_USB_HS )
    MESSAGE(STATUS "Using new CH32 HS Driver")
    SET(CH32V3x_FOLDER ${LN_MCU_FOLDER}/tinyUsb/HS_new)
    INCLUDE_DIRECTORIES(    ${CH32V3x_FOLDER}  )
    SET(DRIVERS
                        ${LNSRC}/lnUsbStack.cpp
                        ${CH32V3x_FOLDER}/dcd_ch32_usbhs.c
                        ${CH32V3x_FOLDER}/dcd_usbhs_platform.cpp
                        ${CH32V3x_FOLDER}/../lnUsbStubs.cpp
                )
  ELSE( )
    MESSAGE(STATUS "Using new CH32 OTG Driver")
    SET(CH32V3x_FOLDER ${LN_MCU_FOLDER}/tinyUsb/FS_new)
    INCLUDE_DIRECTORIES(    ${CH32V3x_FOLDER}  )
    SET(DRIVERS         ${LNSRC}/lnUsbStack.cpp
                        ${CH32V3x_FOLDER}/dcd_ch32_usbfs.c
                        ${CH32V3x_FOLDER}/dcd_usbfs_platform.cpp
                        ${CH32V3x_FOLDER}/../lnUsbStubs.cpp
                )
  ENDIF()

ENDIF()
