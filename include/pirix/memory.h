#pragma once

#include <pirix/types.h>

void memory_init(void);
uintptr_t memory_alloc(void);
uintptr_t memory_alloc_aligned(int frames, int alignment);
void memory_free(uintptr_t frame);
