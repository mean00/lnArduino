SET(FOS ${ARDUINO_GD32_FREERTOS}/FreeRTOS)

INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/common/pico_base${RP_SUFFIX_INCLUDE}/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/common/pico_base_headers/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/ )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2350/pico_platform/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2350/hardware_regs/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2350/hardware_structs/include )

INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/pico_platform_compiler/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/pico_platform_sections/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/pico_platform_panic/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/hardware_sync/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/hardware_base/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/hardware_sync_spin_lock/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/hardware_clocks/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/hardware_exception/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/hardware_irq/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/rp2_common/hardware_timer/include )

INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/common/pico_sync/include )
INCLUDE_DIRECTORIES( ${PICO_SDK_PATH}/src/common/pico_time/include )

SET(RP2350_PORT_DIR ${LN_MCU_FOLDER}/freertos_2350_port)
SET(LN_FREERTOS_PORT ${RP2350_PORT_DIR}/  CACHE INTERNAL "")
SET(LN_FREERTOS_PORT_SOURCES  ${LN_FREERTOS_PORT}/port.c ${LN_FREERTOS_PORT}/portasm.c)


