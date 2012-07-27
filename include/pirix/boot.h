#pragma once

typedef struct boot_header {
    unsigned kernel_start;
    unsigned kernel_end;
} boot_header;

void gpio_init();
void gpio_toggle(int sel, int state);
