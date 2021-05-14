
SET(LONGDUINO ${ARDUINO_GD32_FREERTOS}/legacy/ CACHE INTERNAL "")


# Generate runtime libraries
SET(GD32_ROOT_SRC ${LONGDUINO}/src)
include_directories( ${LONGDUINO}/include)

foreach(periph     gd32vf103_eclic.c n200_func.c            )
    LIST(APPEND GD32_MAIN ${GD32_ROOT_SRC}/${periph})
endforeach()

ADD_LIBRARY(gd32 STATIC  ${GD32_MAIN}  entry.S start.S)


