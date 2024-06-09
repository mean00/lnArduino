configure_file( ${PICO_SDK_PATH}/src/common/pico_base/include/pico/version.h.in ${CMAKE_BINARY_DIR}/pico/version.h)
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/bs2_default_padded_checksummed.S
  COMMAND python ${PICO_SDK_PATH}/src/rp2_common/boot_stage2/pad_checksum -s 0xffffffff ${PICO_SDK_PATH}/src/rp2_common/boot_stage2/bs2_default.bin ${CMAKE_BINARY_DIR}/bs2_default_padded_checksummed.S
  DEPENDS 
  )


IF(USE_RP2040_PURE_RAM)
  SET(CRT0 ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/crt0_ram.S 
           ${CMAKE_CURRENT_SOURCE_DIR}/ln_sys_init_ram.cpp)
ELSE()
  SET(CRT0 ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/crt0.S)
ENDIF()


SET(RP_LN_SOURCES
        ln_rp_gpio.cpp
        ln_rp_dma.cpp
        ln_rp_spi.cpp
)
IF(FALSE)
  SET(BASE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/timer.c # Maybe
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/uart.c # Maybe
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/clocks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/pll.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/vreg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/xosc.c
)
ELSE()
  SET(BASE_SOURCES
    ${PICO_SDK_PATH}/src/rp2_common/hardware_timer/timer.c # Maybe
    ${PICO_SDK_PATH}/src/rp2_common/hardware_uart/uart.c # Maybe
    ${PICO_SDK_PATH}/src/rp2_common/hardware_clocks/clocks.c
    ${PICO_SDK_PATH}/src/rp2_common/hardware_pll/pll.c
    ${PICO_SDK_PATH}/src/rp2_common/hardware_vreg/vreg.c
    ${PICO_SDK_PATH}/src/rp2_common/hardware_xosc/xosc.c
    #${PICO_SDK_PATH}/src/rp2_common/hardware_irq/irq.c
    ${PICO_SDK_PATH}/src/rp2_common/hardware_irq/irq_handler_chain.S
    ${PICO_SDK_PATH}/src/rp2_common/hardware_pio/pio.c
)
ENDIF()
SET(RP_SDK_SOURCES
  ${BASE_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/ln_rp_stubs.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rp_serial.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rp_irq.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/platform.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../sdk_copy/runtime.c
  ${CRT0}
)
add_library(rplib STATIC rp.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../conf/bs2_default_padded_checksummed.S ${RP_LN_SOURCES} ${RP_SDK_SOURCES})
target_include_directories(rplib PUBLIC 
        ${PICO_SDK_PATH}        
        ${PICO_SDK_PATH}/src/boards/include  
        ${PICO_SDK_PATH}/src/common/pico_base/include  
        ${PICO_SDK_PATH}/src/common/pico_binary_info/include  
        ${PICO_SDK_PATH}/src/common/pico_bit_ops/include  
        ${PICO_SDK_PATH}/src/common/pico_divider/include  
        ${PICO_SDK_PATH}/src/common/pico_stdlib/include  
        ${PICO_SDK_PATH}/src/common/pico_sync/include  
        ${PICO_SDK_PATH}/src/common/pico_time/include  
        ${PICO_SDK_PATH}/src/common/pico_util/include 
        ${PICO_SDK_PATH}/src/rp2040/hardware_regs/include  
        ${PICO_SDK_PATH}/src/rp2040/hardware_structs/include  
        ${PICO_SDK_PATH}/src/rp2_common/boot_stage2/include        
        ${PICO_SDK_PATH}/src/rp2_common/hardware_base/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_claim/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_clocks/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_divider/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_irq/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_pll/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_resets/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_sync/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_timer/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_uart/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_vreg/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_watchdog/include  
        ${PICO_SDK_PATH}/src/rp2_common/hardware_xosc/include          
        ${PICO_SDK_PATH}/src/rp2_common/hardware_dma/include       
        ${PICO_SDK_PATH}/src/rp2_common/hardware_pio/include
        ${PICO_SDK_PATH}/src/rp2_common/pico_bootrom/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_double/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_float/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_int64_ops/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_malloc/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_mem_ops/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_platform/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_printf/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_runtime/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_stdio/include  
        ${PICO_SDK_PATH}/src/rp2_common/pico_stdio_uart/include  
        ${PICO_SDK_PATH}/src/rp2_common/boot_stage2/asminclude
  )
target_include_directories(rplib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(rplib  PRIVATE ../private_include )
target_link_libraries(rplib  PRIVATE lnArduinoInternal )

#${PICO_SDK_PATH}/src/rp2_common/pico_platform/platform.c
#${PICO_SDK_PATH}/src/rp2_common/pico_runtime/runtime.c
#${PICO_SDK_PATH}/src/rp2_common/hardware_irq/irq.c

