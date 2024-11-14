include(lnCmake)
SET(S ${CMAKE_CURRENT_SOURCE_DIR})


add_subdirectory(../../common_bluepill common_bp)

target_include_directories(lnArduino_impl PRIVATE ${S} ${S}/../include)

IF(LN_DISABLE_DEFAULT_IRQ_TABLE)

ELSE()
  SET(IRQ_TABLE     ${S}/ch32v3x_interrupt_table.cpp  
                    ${S}/lnIRQ_riscv_ch32v3x.cpp  
      )
ENDIF()

target_sources( lnArduino_impl PRIVATE
    ${S}/lnCpuID_riscv_ch32v3x.cpp  
    ${S}/lnFMC_impl.cpp  
    
    ${IRQ_TABLE}
    ${S}/lnRCU_ch32v3x_usb.cpp  
    ${S}/lnRCU_systemClock_riscv_ch32v3x.cpp  
    ${S}/lnRunTime_riscv_ch32v3x.cpp  
    ${S}/lnSystemTimer_riscv_ch32v3x.cpp  
    ${S}/lnBoot_riscv_ch32.cpp
)
IF(LN_ENABLE_USBD)
        target_sources( lnArduino_impl PRIVATE ${S}/lnUSBD_ch32v3x.cpp)
ENDIF()

IF(LN_ENABLE_I2C)
  target_compile_definitions(lnArduino_impl PRIVATE LN_ENABLE_I2C )
ENDIF()

IF(LN_ENABLE_SPI)
  target_compile_definitions(lnArduino_impl PRIVATE LN_ENABLE_SPI )
ENDIF()

IF(USE_CH32v3x_HW_IRQ_STACK)
  MESSAGE(STATUS "Use WCH fast IRQ")
  target_compile_definitions(lnArduino_impl PRIVATE "-DUSE_CH32v3x_HW_IRQ_STACK")
ELSE()
  MESSAGE(STATUS "Use WCH slow IRQ")  
ENDIF()

