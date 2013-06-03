#pragma once

#include <pirix/types.h>

void kheap_init();
void* kmalloc(size_t size);
void* kcalloc(int count, size_t size);
void kfree(void* addr, size_t size);
