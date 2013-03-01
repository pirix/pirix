#pragma once

void memory_init(void);
unsigned long memory_alloc(void);
unsigned long memory_alloc_aligned(unsigned frames, unsigned alignment);
void memory_free(unsigned long frame);
