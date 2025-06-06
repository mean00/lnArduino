SET(GD32_FOLDER         ${LN_MCU_FOLDER}/tinyUsb/)
SET(DRIVERS             ${LNSRC}/lnUsbStack.cpp ${GD32_FOLDER}/lnTinyUsbImpl.cpp        ${GD32_FOLDER}/lnUsbLL.cpp)
SET(LN_OPT_TUSB_MCU     OPT_MCU_STM32F1)
SET(LN_OPT_MODE         OPT_MODE_FULL_SPEED)
SET(LN_TUSB_EXTRA_DEF   "STM32F103xB=1")

target_compile_definitions(esprit_dev INTERFACE ${LN_TUSB_EXTRA_DEF})
