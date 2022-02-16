SET(FOS ${ARDUINO_GD32_FREERTOS}/FreeRTOS)
include( ${ARDUINO_GD32_FREERTOS}/freeRTOS_port_${LN_EXT}.cmake)

FOREACH(common croutine.c  event_groups.c  list.c  queue.c  stream_buffer.c  tasks.c  timers.c)
    LIST(APPEND CMN ${FOS}/${common})
ENDFOREACH(common croutine.c  event_groups.c  list.c  queue.c  stream_buffer.c  tasks.c  timers.c)

ADD_DEFINITIONS("-DLN_FREERTOS_HEAP_SIZE=(${LN_MCU_RAM_SIZE}-${LN_MCU_STATIC_RAM})")

SET(MEM ${FOS}/portable/MemMang/heap_4.c)

ADD_LIBRARY( FreeRTOS STATIC ${CMN} ${LN_FREERTOS_PORT_SOURCES} ${MEM})
target_include_directories(FreeRTOS PRIVATE ${FOS}/..)
target_include_directories(FreeRTOS PRIVATE ${FOS}/../legacy/include)
target_include_directories(FreeRTOS PRIVATE ${FOS}/include)
target_include_directories(FreeRTOS PRIVATE ${LN_FREERTOS_PORT})
target_include_directories(FreeRTOS PRIVATE ${CMAKE_SOURCE_DIR})

include_directories(${ARDUINO_GD32_FREERTOS}/legacy/boards/${GD32_BOARD}/)
