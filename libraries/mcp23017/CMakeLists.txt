
SET(SRCS mcp23017.cpp )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(mcp23017 STATIC ${SRCS})
target_include_directories(mcp23017 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
