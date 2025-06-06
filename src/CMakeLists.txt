#

ADD_LIBRARY(ln_utils STATIC)
TARGET_SOURCES( ln_utils PRIVATE
                       lnFreeRTOS_pp.cpp
                       lnDebug.cpp
                       systemHelper.cpp
                       lnStopWatch.cpp)
IF(LN_EXTERNAL_MALLOC)
ELSE()
  TARGET_SOURCES( ln_utils PRIVATE
                       systemAlloc.cpp
                       ln_libc.cpp
                    )
ENDIF()
TARGET_LINK_LIBRARIES(ln_utils PRIVATE  esprit_dev)


If(LN_DEBUG_UART)
  MESSAGE(STATUS "Using uart ${LN_DEBUG_UART} for debug")
  TARGET_COMPILE_DEFINITIONS(ln_utils PRIVATE LN_DEBUG_UART=${LN_DEBUG_UART})
ENDIF()
