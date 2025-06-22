SET(FOS ${ESPRIT_ROOT}/FreeRTOS CACHE INTERNAL "")
include( ${LN_MCU_FOLDER}/freeRTOS_port_${LN_EXT}.cmake)

FOREACH(common   event_groups.c  list.c  queue.c  stream_buffer.c  tasks.c  timers.c)
  LIST(APPEND CMN ${FOS}/${common})
ENDFOREACH(common   event_groups.c  list.c  queue.c  stream_buffer.c  tasks.c  timers.c)

ADD_DEFINITIONS("-DLN_FREERTOS_HEAP_SIZE=(${LN_MCU_RAM_SIZE}-${LN_MCU_STATIC_RAM}-1)")

SET(MEM ${FOS}/portable/MemMang/heap_4.c)
#
SET(FOS_SOURCES ${CMN} ${LN_FREERTOS_PORT_SOURCES} ${MEM} CACHE INTERNAL "")
MESSAGE(STATUS "Using internal freertos with config in ${ESPRIT_ROOT}/freertos_config for target ${LN_EXT} boards ${LN_BOARD_NAME}")
#
target_include_directories(esprit_dev INTERFACE ${ESPRIT_ROOT}/freertos_config)
target_include_directories(esprit_dev INTERFACE ${LN_FREERTOS_PORT})
target_include_directories(esprit_dev INTERFACE ${CMAKE_SOURCE_DIR})
target_include_directories(esprit_dev INTERFACE ${FOS}/)
target_include_directories(esprit_dev INTERFACE ${FOS}/..)
target_include_directories(esprit_dev INTERFACE ${FOS}/include)
#
ADD_LIBRARY( FreeRTOS OBJECT)
target_sources(FreeRTOS PRIVATE ${FOS_SOURCES})
target_link_libraries(FreeRTOS PUBLIC esprit_dev)
#

