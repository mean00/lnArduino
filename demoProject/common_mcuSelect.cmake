IF(NOT DEFINED LN_ARCH)
IF(1)
    SET(LN_ARCH "ARM" CACHE INTERNAL "")
    SET(LN_MCU  "M3" CACHE INTERNAL "")
ELSE()
    SET(LN_ARCH "RISCV" CACHE INTERNAL "")
    SET(LN_MCU  "VF103" CACHE INTERNAL "")
ENDIF()
ENDIF(NOT DEFINED LN_ARCH)
MESSAGE(STATUS "Architecture ${LN_ARCH}, MCU=${LN_MCU}")
