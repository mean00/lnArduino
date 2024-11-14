SET(EPR ${ARDUINO_GD32_FREERTOS}/embedded_printf/)
include_directories( ${EPR})

FOREACH(common printf.c)
    LIST(APPEND EPS ${EPR}/${common})
ENDFOREACH(common printf.c)

IF(LN_DISABLE_FLOAT_PRINTF)
    SET(embedded_printf_float "-DPRINTF_DISABLE_SUPPORT_FLOAT" CACHE INTERNAL "")
ELSE()
    SET(embedded_printf_float "")
ENDIF()


ADD_LIBRARY( embeddedPrintf STATIC ${EPS})
target_include_directories(embeddedPrintf PUBLIC ${EPR})
target_compile_options(embeddedPrintf PRIVATE ${embedded_printf_float} -DPRINTF_DISABLE_SUPPORT_DOUBLE -DPRINTF_DISABLE_SUPPORT_EXPONENTIAL -DPRINTF_DISABLE_SUPPORT_LONG_LONG -DPRINTF_DISABLE_SUPPORT_PTRDIFF_T) 
