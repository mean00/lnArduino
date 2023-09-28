SET(CH32V3x_FOLDER ${LN_MCU_FOLDER}/tinyUsb/)
SET(DRIVERS             ${LNSRC}/lnTinyUsbImpl.cpp        ${LNSRC}/lnUsbStack.cpp ${LNSRC}/lnUsbLL.cpp)
SET(LN_OPT_TUSB_MCU     OPT_MCU_STM32F1)
SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)              
#target_compile_definitions(tinyUsb PUBLIC STM32F103xB ) # This keeps tinyUsb happy
   
