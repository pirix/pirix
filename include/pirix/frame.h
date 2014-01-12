#pragma once

#include <pirix/types.h>

void frame_init(void);
uintptr_t frame_alloc(void);
uintptr_t frame_alloc_aligned(int frames, int alignment);
void frame_free(uintptr_t frame);
