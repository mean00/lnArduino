SET(FOS ${ARDUINO_GD32_FREERTOS}/FreeRTOS)


FOREACH(common croutine.c  event_groups.c  list.c  queue.c  stream_buffer.c  tasks.c  timers.c)
    LIST(APPEND CMN ${FOS}/${common})
ENDFOREACH(common croutine.c  event_groups.c  list.c  queue.c  stream_buffer.c  tasks.c  timers.c)

#SET(PORT ${FOS}/portable/GCC/RISC-V)
SET(PORT ${ARDUINO_GD32_FREERTOS}/freeRTOS_extension/N200)

SET(MEM ${FOS}/portable/MemMang/heap_4.c)

ADD_LIBRARY( FreeRTOS STATIC ${CMN} ${PORT}/port.c ${PORT}/portASM.S ${MEM})
target_include_directories(FreeRTOS PRIVATE ${FOS}/..)
target_include_directories(FreeRTOS PRIVATE ${FOS}/../legacy/include)
target_include_directories(FreeRTOS PRIVATE ${FOS}/include)
target_include_directories(FreeRTOS PRIVATE ${PORT})
target_include_directories(FreeRTOS PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(FreeRTOS PRIVATE ${CMAKE_SOURCE_DIR}/overlay/freeRTOS_extension)
