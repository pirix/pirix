#include <pirix/memory.h>
#include <pirix/string.h>

#define BITMAP_SIZE 32768

static unsigned bitmap[BITMAP_SIZE];

#pragma weak memory_init
void memory_init() {
   memset(bitmap+64, 0xff, sizeof(unsigned[BITMAP_SIZE-64]));
}

unsigned long memory_alloc() {
    for (unsigned i = 0; i < BITMAP_SIZE; i++) {
        // skip this element if full
        if (!bitmap[i]) continue;

        // check each bit for a free section
        for (unsigned j = 0; j < 32; j++) {
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

unsigned long memory_alloc_aligned(unsigned frames, unsigned alignment) {
    unsigned align_val = 1 << alignment;
    unsigned align_mask = align_val - 1;

    for (unsigned i = 0; i < BITMAP_SIZE; i++) {
        if (!bitmap[i]) continue;

        unsigned start = (i*32) & align_mask;
        for (unsigned j = start; j <= 32 - frames; j += align_val) {
            if (!(bitmap[i] & (1 << j))) continue;

            unsigned k;
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

void memory_free(unsigned long frame) {
    unsigned long f = frame/0x1000;
    bitmap[f/32] |= 1 << (f % 32);
}
