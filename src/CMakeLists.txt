#
SET(LN_SOURCE_UTILS       
                       lnFreeRTOS_pp.cpp 
                       systemAlloc.cpp 
                       lnDebug.cpp
                       lnStopWatch.cpp
                       systemHelper.cpp
                    )

ADD_LIBRARY(ln_utils STATIC  ${LN_SOURCE_UTILS} )
TARGET_LINK_LIBRARIES(ln_utils PRIVATE  lnArduinoInternal)


If(LN_DEBUG_UART)
  MESSAGE(STATUS "Using uart ${LN_DEBUG_UART} for debug")
  TARGET_COMPILE_DEFINITIONS(ln_utils PRIVATE LN_DEBUG_UART=${LN_DEBUG_UART})
ENDIF()
