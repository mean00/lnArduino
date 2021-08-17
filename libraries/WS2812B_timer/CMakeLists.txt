
SET(SRCS lnWS2812B.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(WS2812B_timer STATIC ${SRCS})
target_include_directories(WS2812B_timer PRIVATE ${CMAKE_CURRENT_SOURCe_DIR})
