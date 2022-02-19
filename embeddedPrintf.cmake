SET(EPR ${ARDUINO_GD32_FREERTOS}/embedded_printf/)
include_directories( ${EPR})

FOREACH(common printf.c)
    LIST(APPEND EPS ${EPR}/${common})
ENDFOREACH(common printf.c)



ADD_LIBRARY( embeddedPrintf STATIC ${EPS})
target_compile_options(embeddedPrintf PRIVATE -DPRINTF_DISABLE_SUPPORT_EXPONENTIAL -DPRINTF_DISABLE_SUPPORT_LONG_LONG -DPRINTF_DISABLE_SUPPORT_PTRDIFF_T) 
