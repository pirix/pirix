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
elseif(PLATFORM STREQUAL i386)
  set(PLATFORM_SRC
    ../drivers/serial/i386.c
    ../drivers/timer/i386.c
    ../drivers/irq/i386.c
  )
endif(PLATFORM STREQUAL RBPI)

if(ARCH STREQUAL arm)
  set(ARCH_SRC
    arm/init.S
    arm/paging.c
    arm/div.c
  )
elseif(ARCH STREQUAL i386)
  set(ARCH_SRC
    i386/init.S
    i386/irq.S
    i386/gdt.S
    i386/paging.c
  )
endif(ARCH STREQUAL arm)

add_executable(kernel
  main.c
  memory.c
  kheap.c
  irq.c
  timer.c
  process.c
  thread.c
  syscall.c
  kprint.c
  ipc.c
  scheduler.c
  string.c
  vector.c
  ${ARCH_SRC}
  ${PLATFORM_SRC}
)

set_target_properties(kernel PROPERTIES
  LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/${ARCH}/link.ld"
)

install(TARGETS kernel RUNTIME DESTINATION bin)