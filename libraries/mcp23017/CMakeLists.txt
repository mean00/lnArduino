
SET(SRCS mcp23017_button.cpp  mcp23017.cpp  mcp23017_rotary.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(mcp23017 STATIC ${SRCS})
target_include_directories(mcp23017 PRIVATE ${CMAKE_CURRENT_SOURCe_DIR})
