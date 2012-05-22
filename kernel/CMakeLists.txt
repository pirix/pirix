add_library(kernel OBJECT
  init.s
  main.c
  paging.c
  paging.s
  memory.c
  kheap.c
  irq.s
  irq.c
  timer.c
  task.c
  syscall.s
  syscall.c
  fs.c
  div.c
  serial.c
  initrd.c
)