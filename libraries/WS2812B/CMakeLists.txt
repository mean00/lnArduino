
SET(SRCS lnWS2812B.cpp lnWS2812B_timer.cpp lnWS2812B_base.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(WS2812B STATIC ${SRCS})
target_include_directories(WS2812B PRIVATE ${CMAKE_CURRENT_SOURCe_DIR})
target_link_libraries(WS2812B PRIVATE esprit_dev)
