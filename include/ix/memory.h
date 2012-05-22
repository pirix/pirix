#pragma once

void memory_init(void);
unsigned memory_alloc(void);
unsigned memory_alloc_aligned(unsigned frames, unsigned alignment);
void memory_free(unsigned frame);
