if(PLATFORM STREQUAL RBPI)
  set(PLATFORM_SRC
    ../drivers/serial/bcm2835.c
    ../drivers/timer/bcm2835.c
    ../drivers/irq/bcm2835.c
  )
elseif(PLATFORM STREQUAL QEMU)
  set(PLATFORM_SRC
    ../drivers/serial/pl011.c
    ../drivers/timer/icp.c
    ../drivers/irq/icp.c
  )
endif(PLATFORM STREQUAL RBPI)

add_executable(kernel
  init.S
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
  ${PLATFORM_SRC}
)

target_link_libraries(kernel util system)

set_source_files_properties(init.S PROPERTIES LANGUAGE C)

set_target_properties(kernel PROPERTIES
  LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/tools/kernel.ld"
)
