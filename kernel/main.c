#include <ix/task.h>
#include <ix/irq.h>
#include <ix/memory.h>
#include <ix/paging.h>

void panic() {
  serial_puts("\nKernel Panic!\n");
  for (;;);
}

void main() {
  serial_puts("IX VERSION 0.1 BOOTING...\n\n");

  serial_puts(":: Init Memory\n");
  memory_init();

  serial_puts(":: Init Paging\n");
  paging_init();

  initrd_load();

  serial_puts(":: Init Interrupts\n");
  irq_init();

  serial_puts(":: Start Timer\n");
  timer_init();

  for (;;);
}
