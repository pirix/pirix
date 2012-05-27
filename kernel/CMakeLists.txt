add_library(kernel OBJECT
  init.s
  main.c
  mem/paging.c
  mem/paging.s
  mem/memory.c
  mem/kheap.c
  dev/irq.s
  dev/irq.c
  dev/timer.c
  sys/task.c
  sys/syscall.s
  sys/syscall.c
  sys/kprint.c
  fs/fs.c
  fs/file.c
  fs/initrd.c
)