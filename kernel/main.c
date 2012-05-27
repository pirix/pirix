#include <pirix/kprint.h>
#include <pirix/video.h>
#include <pirix/timer.h>
#include <pirix/irq.h>
#include <pirix/memory.h>
#include <pirix/paging.h>
#include <pirix/fs.h>
#include <pirix/initrd.h>
#include <elf.h>

void panic(const char* text) {
    kputs("\n\nKERNEL PANIC!\n");
    kputs(text);
    kputs("\nPlease reboot the system.");
    for (;;);
}

static void init(const char* name, int (*handler)()) {
    kprintf(":: %s \t", name);
    int res = handler();
    if (res) kputs("[fail]\n");
    else kputs("[ok]\n");
}

void main() {
    kputs("PIRIX VERSION 0.1 BOOTING...\n\n");

    init("video", &video_init);
    init("memory", &memory_init);
    init("paging", &paging_init);
    init("filesystem", &fs_init);
    init("initrd", &initrd_init);
    init("irqs", &irq_init);
    init("timer", &timer_init);

    fs_node* f = fs_dir_read(fs_root, "user/init");
    char buff[2000];
    elf_init(fs_read(f, );
    kputs("p");

    kputs("\nReady.");

    for (;;);
}
