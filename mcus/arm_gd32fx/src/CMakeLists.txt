SET(S ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(../../common_bluepill common_bp)
target_include_directories(lnArduino_impl PRIVATE ${S})
target_sources( lnArduino_impl PRIVATE
        ${S}/lnCpuID_arm_gd32fx.cpp     ${S}/lnFMC_impl.cpp     
        ${S}/lnIRQ_arm_gd32fx.cpp   ${S}/lnRCU_systemClock_arm_gd32fx.cpp   
        ${S}/lnRunTime_arm_gd32fx.cpp   ${S}/lnSystemTimer_arm_gd32fx.cpp   
        ${S}/start.cpp
)
IF(LN_ENABLE_USBD)
        target_sources( lnArduino_impl PRIVATE ${S}/lnUSBD.cpp)
ENDIF()
