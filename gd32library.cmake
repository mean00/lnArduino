
SET(LONGDUINO ${ARDUINO_GD32_FREERTOS}/Longduino_cmake CACHE INTERNAL "")


# Generate runtime libraries
SET(GD32SK ${LONGDUINO}/cores/arduino//)
SET(GD32_ROOT_SRC ${GD32SK}/GD32VF103_Firmware_Library)

SET(GD32_RISC ${GD32_ROOT_SRC}/RISCV)
include_directories( ${GD32_RISC}/drivers)
SET(RISC_SRC ${GD32_RISC}/drivers/n200_func.c)


SET(GD32_STD ${GD32_ROOT_SRC}/GD32VF103_standard_peripheral)
include_directories( ${GD32_STD})
include_directories( ${GD32_STD}/Include)

foreach(periph gd32vf103_adc.c  gd32vf103_can.c  gd32vf103_dac.c  gd32vf103_dma.c    gd32vf103_exmc.c  gd32vf103_fmc.c    gd32vf103_gpio.c  
               gd32vf103_pmu.c  gd32vf103_rtc.c  gd32vf103_timer.c          gd32vf103_wwdgt.c gd32vf103_bkp.c  gd32vf103_crc.c  gd32vf103_dbg.c  
               gd32vf103_eclic.c  gd32vf103_exti.c  gd32vf103_fwdgt.c  gd32vf103_i2c.c   gd32vf103_rcu.c  gd32vf103_spi.c  gd32vf103_usart.c               
            )
    LIST(APPEND GD32_MAIN ${GD32_STD}/Source/${periph})
endforeach()
#MESSAGE(STATUS "Peripherals : ${p}")
    
foreach(sys system_gd32vf103.c)   
     LIST(APPEND GD32_MAIN ${GD32_STD}/${sys})
endforeach(sys system_gd32vf103.c)   
    


foreach(misc  init.c handlers.c )#entry.S start.S)
    LIST(APPEND GD32_MISC  ${GD32_ROOT_SRC}/RISCV/env_Eclipse/${misc})
endforeach()


foreach(stubs close.c  _exit.c  fstat.c  isatty.c  lseek.c  read.c  sbrk.c  stub.h  write.c  write_hex.c)
    LIST(APPEND GD32_STUBS ${GD32_ROOT_SRC}/RISCV/stubs/${stubs})
endforeach()


ADD_LIBRARY(gd32 STATIC  ${GD32_MAIN} ${GD32_STUBS} ${RISC_SRC} ${GD32_MISC} entry.S start.S)
TARGET_INCLUDE_DIRECTORIES(gd32 PRIVATE ${CMAKE_SOURCE_DIR}/overlay/include)
add_subdirectory(src)
