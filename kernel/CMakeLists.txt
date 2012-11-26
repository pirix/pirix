add_executable(kernel
  init.s
  main.c
  paging.c
  memory.c
  kheap.c
  irq.c
  timer.c
  process.c
  thread.c
  syscall.c
  kprint.c
  ../drivers/serial/bcm2835.c
  gpio.c
)

target_link_libraries(kernel util system)

set_target_properties(kernel PROPERTIES
  LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/tools/kernel.ld"
)
