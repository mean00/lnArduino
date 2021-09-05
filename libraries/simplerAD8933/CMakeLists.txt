
SET(SRCS simplerAD8933.cpp )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(simplerAD8933 STATIC ${SRCS})
target_include_directories(simplerAD8933 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
