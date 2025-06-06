include(lnCmake)
SET(S ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(../../common_bluepill common_bp)
target_include_directories(Esprit_impl PRIVATE ${S})
target_include_directories(lnBoard  INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/../../common_bluepill/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../boards/${LN_BOARD_NAME})
target_sources( Esprit_impl PRIVATE
        ${S}/lnCpuID_arm_gd32fx.cpp     ${S}/lnFMC_impl.cpp
        ${S}/lnIRQ_arm_gd32fx.cpp   ${S}/lnRCU_systemClock_arm_gd32fx.cpp
        ${S}/lnRunTime_arm_gd32fx.cpp   ${S}/lnSystemTimer_arm_gd32fx.cpp
        ${S}/start.cpp
        ${S}/../boards/${LN_BOARD_NAME}/lnPinMapping.cpp
)
IF(LN_ENABLE_USBD)
  target_sources( Esprit_impl PRIVATE ${S}/lnUSBD.cpp)
ENDIF()


IF(LN_ENABLE_I2C)
  target_compile_definitions(Esprit_impl PRIVATE LN_ENABLE_I2C )
ENDIF()

IF(LN_ENABLE_SPI)
  target_compile_definitions(Esprit_impl PRIVATE LN_ENABLE_SPI )
ENDIF()



#  LN_APPEND_FLAGS( ${S}/lnIRQ_arm_gd32fx.cpp -DLN_ENABLE_I2C)
#ENDIF(LN_ENABLE_I2C)

#IF(LN_ENABLE_SPI)
#  LN_APPEND_FLAGS(  ${S}/lnIRQ_arm_gd32fx.cpp -DLN_ENABLE_SPI)
#ENDIF(LN_ENABLE_SPI)
