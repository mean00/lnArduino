include(lnCmake)
SET(S ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(../../common_bluepill common_bp)

target_include_directories(lnArduino_impl PRIVATE ${S} ${S}/../include)

target_sources( lnArduino_impl PRIVATE
    ${S}/lnCpuID_riscv_ch32v3x.cpp  
    ${S}/lnFMC_impl.cpp  
    ${S}/lnIRQ_riscv_ch32v3x.cpp  
    ${S}/lnRCU_ch32v3x.cpp  
    ${S}/lnRCU_systemClock_riscv_ch32v3x.cpp  
    ${S}/lnRunTime_riscv_ch32v3x.cpp  
    ${S}/lnSystemTimer_riscv_ch32v3x.cpp  
)
IF(LN_ENABLE_USBD)
        target_sources( lnArduino_impl PRIVATE ${S}/lnUSBD_ch32v3x.cpp)
ENDIF()


IF(LN_ENABLE_I2C)
  LN_APPEND_FLAGS( ${S}/lnIRQ_riscv_ch32v3x.cpp -DLN_ENABLE_I2C)
ENDIF(LN_ENABLE_I2C)

IF(LN_ENABLE_SPI)  
  LN_APPEND_FLAGS(  ${S}/lnIRQ_riscv_ch32v3x.cpp -DLN_ENABLE_SPI)
ENDIF(LN_ENABLE_SPI)
