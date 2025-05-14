SET(FOS ${LNARDUINO_ROOT}/FreeRTOS)
# Enable saving fpu regs ?
IF(USE_CH32v3x_HW_IRQ_STACK)
    ADD_DEFINITIONS("-DUSE_CH32v3x_HW_IRQ_STACK=1")
ENDIF()
IF(USE_HW_FPU)
    ADD_DEFINITIONS("-DARCH_FPU=1")
else()
    ADD_DEFINITIONS("-DARCH_FPU=0")
ENDIF()
# WCH version
SET(LN_FREERTOS_PORT ${LN_MCU_FOLDER}/freeRTOS_extension/ CACHE INTERNAL "")
include_directories( ${LN_FREERTOS_PORT}/ )
include_directories( ${LN_MCU_FOLDER}/ )
include_directories( ${LN_FREERTOS_PORT}/chip_specific_extensions/RV32I_PFIC_no_extensions )
SET(LN_FREERTOS_PORT_SOURCES  ${LN_FREERTOS_PORT}/port.c ${LN_FREERTOS_PORT}/portASM.S   )


