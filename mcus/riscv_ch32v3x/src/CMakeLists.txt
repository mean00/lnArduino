add_subdirectory(../../common_bluepill common_bp)
SET(S ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(lnArduino_impl PRIVATE ${S})
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
