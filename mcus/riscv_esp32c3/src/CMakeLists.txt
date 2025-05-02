include(lnCmake)
SET(S ${CMAKE_CURRENT_SOURCE_DIR})


add_subdirectory(../../common_bluepill common_bp)

target_include_directories(lnArduino_impl PRIVATE ${S} ${S}/../include)

target_sources( lnArduino_impl PRIVATE
)
IF(LN_ENABLE_USBD)
ENDIF()

IF(LN_ENABLE_I2C)
  target_compile_definitions(lnArduino_impl PRIVATE LN_ENABLE_I2C )
ENDIF()

IF(LN_ENABLE_SPI)
  target_compile_definitions(lnArduino_impl PRIVATE LN_ENABLE_SPI )
ENDIF()


