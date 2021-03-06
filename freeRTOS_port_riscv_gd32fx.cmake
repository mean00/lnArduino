SET(FOS ${ARDUINO_GD32_FREERTOS}/FreeRTOS)
IF(0)
    # Nuclei version
    SET(LN_FREERTOS_PORT ${ARDUINO_GD32_FREERTOS}/riscv_gd32fx/freeRTOS_extension/N200_nuclei CACHE INTERNAL "")
    SET(LN_FREERTOS_PORT_SOURCES  ${LN_FREERTOS_PORT}/port.c ${LN_FREERTOS_PORT}/portasm.S   ${LN_FREERTOS_PORT}/port_interrupt.c ${LN_FREERTOS_PORT}/trap.S)
ELSE()
    # QQ version
    SET(LN_FREERTOS_PORT ${ARDUINO_GD32_FREERTOS}/riscv_gd32fx/freeRTOS_extension/N200 CACHE INTERNAL "")
    SET(LN_FREERTOS_PORT_SOURCES  ${LN_FREERTOS_PORT}/port.c ${LN_FREERTOS_PORT}/portASM.S ${LN_FREERTOS_PORT}/entry.S)
ENDIF()


