#include <pirix/memory.h>
#include <pirix/string.h>
#include <pirix/kernel.h>

#define BITMAP_SIZE 32768

static uint32_t bitmap[BITMAP_SIZE];

void memory_init() {
   memset(bitmap+64, 0xff, sizeof(uint32_t[BITMAP_SIZE-64]));
}

uintptr_t memory_alloc() {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        // skip this element if full
        if (!bitmap[i]) continue;

        // check each bit for a free section
        for (int j = 0; j < 32; j++) {
            // skip if not free
            if (!(bitmap[i] & (1 << j))) continue;

            // found a free section
            bitmap[i] &= ~(1 << j);
            return (i*32 + j)*0x1000;
        }
    }

    panic("out of memory");
    return 0;
}

uintptr_t memory_alloc_aligned(int frames, int alignment) {
    int align_val = 1 << alignment;
    int align_mask = align_val - 1;

    for (int i = 0; i < BITMAP_SIZE; i++) {
        if (!bitmap[i]) continue;

        int start = (i*32) & align_mask;
        for (int j = start; j <= 32 - frames; j += align_val) {
            if (!(bitmap[i] & (1 << j))) continue;

            int k;
            for (k = 0; (k < frames) && (bitmap[i] & (1 << (j+k))); k++);

            if (k < frames) continue;

            for (k = 0; k < frames; k++) {
                bitmap[i] &= ~(1 << (j+k));
            }

            return (i*32+j)*0x1000;
        }
    }

    panic("out of memory");
    return 0;
}

void memory_free(uintptr_t frame) {
    uintptr_t f = frame/0x1000;
    bitmap[f/32] |= 1 << (f % 32);
}
