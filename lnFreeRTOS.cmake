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
ADD_LIBRARY( FreeRTOS STATIC)
target_sources(FreeRTOS PRIVATE ${FOS_SOURCES})
target_include_directories(FreeRTOS PUBLIC ${ESPRIT_ROOT}/freertos_config)
target_include_directories(FreeRTOS PUBLIC ${LN_FREERTOS_PORT})
target_include_directories(FreeRTOS PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(FreeRTOS PUBLIC ${FOS}/)
target_include_directories(FreeRTOS PUBLIC ${FOS}/..)
target_include_directories(FreeRTOS PUBLIC ${FOS}/include)
target_link_libraries(FreeRTOS PUBLIC esprit_dev)
#

