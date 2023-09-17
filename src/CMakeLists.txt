#
SET(LN_SOURCE_UTILS       
                       MapleFreeRTOS1000_pp.cpp 
                       systemAlloc.cpp 
                       lnDebug.cpp
                       lnStopWatch.cpp
                       lnFreeRTOSDebug.cpp
                       systemHelper.cpp
                    )

ADD_LIBRARY(ln_utils STATIC  ${LN_SOURCE_UTILS} )
TARGET_LINK_LIBRARIES(ln_utils PRIVATE  lnArduinoInternal)


