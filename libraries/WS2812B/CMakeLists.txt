
SET(SRCS lnWS2812B.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(WS2812B STATIC ${SRCS})
target_include_directories(WS2812B PRIVATE ${CMAKE_CURRENT_SOURCe_DIR})
