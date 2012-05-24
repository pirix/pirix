#pragma once

#define WORD_SIZE (sizeof(unsigned))
#define WORD_BITS (WORD_SIZE*8)

void memory_init(void);
unsigned memory_alloc(void);
unsigned memory_alloc_aligned(unsigned frames, unsigned alignment);
void memory_free(unsigned frame);
