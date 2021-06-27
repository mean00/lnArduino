
SET(SRCS RotaryEncoder.cpp  Rotary/Rotary.cpp )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Rotary)
add_library(RotaryEncoder ${SRCS} )
