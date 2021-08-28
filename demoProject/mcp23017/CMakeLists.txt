cmake_minimum_required(VERSION 3.0)

include(../common_CMakeLists.cmake)
PROJECT(Longan_mcp23017 C CXX ASM)

include_directories(${CMAKE_SOURCE_DIR})        
add_subdirectory(${AF_FOLDER})


include(${AF_FOLDER}/setup.cmake)
        
#generic MCP part
SET( MCP23017 ${AF_FOLDER}/libraries/mcp23017 )
add_subdirectory(${MCP23017})
INCLUDE_DIRECTORIES( ${MCP23017} ${MCP23017}/)

# longan nano specific part
INCLUDE_DIRECTORIES( ${MCP23017} ${MCP23017}/platformExample/longanNano)
SET(LIBMCP23017  ${MCP23017}/platformExample/longanNano/lnMcp23017.cpp)
#        
GENERATE_GD32_FIRMWARE(demo_mcp23017 hello.cpp ${LIBMCP23017} )       
target_link_libraries(demo_mcp23017 mcp23017)
