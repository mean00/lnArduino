configure_file( ${PICO_SDK_PATH}/src/common/pico_base/include/pico/version.h.in ${CMAKE_BINARY_DIR}/pico/version.h)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/bs2_default_padded_checksummed.S
  COMMAND python ${PICO_SDK_PATH}/src/rp2_common/boot_stage2/pad_checksum -s 0xffffffff ${PICO_SDK_PATH}/src/rp2_common/boot_stage2/bs2_default.bin ${CMAKE_BINARY_DIR}/bs2_default_padded_checksummed.S
  DEPENDS 
  )


SET(RP_SDK_SOURCES
${PICO_SDK_PATH}/src/common/pico_sync/critical_section.c
${PICO_SDK_PATH}/src/common/pico_sync/lock_core.c
${PICO_SDK_PATH}/src/common/pico_sync/mutex.c
${PICO_SDK_PATH}/src/common/pico_sync/sem.c
${PICO_SDK_PATH}/src/common/pico_time/time.c
${PICO_SDK_PATH}/src/common/pico_time/timeout_helper.c
${PICO_SDK_PATH}/src/common/pico_util/datetime.c
${PICO_SDK_PATH}/src/common/pico_util/pheap.c
${PICO_SDK_PATH}/src/common/pico_util/queue.c
${PICO_SDK_PATH}/src/rp2_common/boot_stage2/compile_time_choice.S
${PICO_SDK_PATH}/src/rp2_common/hardware_claim/claim.c
${PICO_SDK_PATH}/src/rp2_common/hardware_clocks/clocks.c
${PICO_SDK_PATH}/src/rp2_common/hardware_divider/divider.S
${PICO_SDK_PATH}/src/rp2_common/hardware_gpio/gpio.c
${PICO_SDK_PATH}/src/rp2_common/hardware_irq/irq.c
${PICO_SDK_PATH}/src/rp2_common/hardware_irq/irq_handler_chain.S
${PICO_SDK_PATH}/src/rp2_common/hardware_pll/pll.c
${PICO_SDK_PATH}/src/rp2_common/hardware_sync/sync.c
${PICO_SDK_PATH}/src/rp2_common/hardware_timer/timer.c
${PICO_SDK_PATH}/src/rp2_common/hardware_uart/uart.c
${PICO_SDK_PATH}/src/rp2_common/hardware_vreg/vreg.c
${PICO_SDK_PATH}/src/rp2_common/hardware_watchdog/watchdog.c
${PICO_SDK_PATH}/src/rp2_common/hardware_xosc/xosc.c
${PICO_SDK_PATH}/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S
${PICO_SDK_PATH}/src/rp2_common/pico_bootrom/bootrom.c
${PICO_SDK_PATH}/src/rp2_common/pico_divider/divider.S
${PICO_SDK_PATH}/src/rp2_common/pico_double/double_aeabi.S
${PICO_SDK_PATH}/src/rp2_common/pico_double/double_init_rom.c
${PICO_SDK_PATH}/src/rp2_common/pico_double/double_math.c
${PICO_SDK_PATH}/src/rp2_common/pico_double/double_v1_rom_shim.S
${PICO_SDK_PATH}/src/rp2_common/pico_float/float_aeabi.S
${PICO_SDK_PATH}/src/rp2_common/pico_float/float_init_rom.c
${PICO_SDK_PATH}/src/rp2_common/pico_float/float_math.c
${PICO_SDK_PATH}/src/rp2_common/pico_float/float_v1_rom_shim.S
${PICO_SDK_PATH}/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S
${PICO_SDK_PATH}/src/rp2_common/pico_malloc/pico_malloc.c
${PICO_SDK_PATH}/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S
${PICO_SDK_PATH}/src/rp2_common/pico_platform/platform.c
${PICO_SDK_PATH}/src/rp2_common/pico_printf/printf.c
${PICO_SDK_PATH}/src/rp2_common/pico_runtime/runtime.c
${PICO_SDK_PATH}/src/rp2_common/pico_standard_link/binary_info.c
${PICO_SDK_PATH}/src/rp2_common/pico_standard_link/new_delete.cpp
${PICO_SDK_PATH}/src/rp2_common/pico_stdio/stdio.c
${PICO_SDK_PATH}/src/rp2_common/pico_stdio_uart/stdio_uart.c
${PICO_SDK_PATH}/src/rp2_common/pico_stdlib/stdlib.c
${PICO_SDK_PATH}/src/rp2_common/pico_standard_link/crt0.S


${CMAKE_CURRENT_SOURCE_DIR}/rp_gpio.cpp
${CMAKE_CURRENT_SOURCE_DIR}/rp_serial.cpp
${CMAKE_CURRENT_SOURCE_DIR}/rp_irq.cpp


)


add_library(rplib STATIC rp.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../conf/bs2_default_padded_checksummed.S ${RP_SDK_SOURCES})
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
        ${PICO_SDK_PATH}/src/rp2_common/boot_stage2/include -g3   
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
#target_include_directories(rplib PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(rplib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(rplib  PRIVATE lnArduinoInternal )            
#target_link_libraries(lnArduino PUBLIC rplib )