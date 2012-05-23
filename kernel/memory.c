#include <ix/memory.h>
#include <string.h>

#define BITMAP_SIZE 1024

static unsigned bitmap[BITMAP_SIZE];

void memory_init(void) {
    memset(bitmap+16, 0xff, (BITMAP_SIZE-16)*WORD_SIZE);
}

unsigned memory_alloc(void) {
    for (unsigned i = 0; i < BITMAP_SIZE; i++) {
        // skip this element if full
        if (!bitmap[i]) continue;

        // check each bit for a free section
        for (unsigned j = 0; j < WORD_BITS; j++) {
            // skip if not free
            if (!(bitmap[i] & (1 << j))) continue;

            // found a free section
            bitmap[i] &= ~(1 << j);
            return (i*WORD_BITS + j)*4096;
        }
    }
    return 0;
}

unsigned memory_alloc_aligned(unsigned frames, unsigned alignment) {
    unsigned align_val = 1 << alignment;
    unsigned align_mask = align_val - 1;

    for (unsigned i = 0; i < BITMAP_SIZE; i++) {
        if (!bitmap[i]) continue;

        unsigned start = (i*WORD_BITS) & align_mask;
        for (unsigned j = start; j <= WORD_BITS - frames; j += align_val) {
            if (!(bitmap[i] & (1 << j))) continue;

            unsigned k;
            for (k = 0; (k < frames) && (bitmap[i] & (1 << (j+k))); k++);

            if (k < frames) continue;

            for (k = 0; k < frames; k++) {
                bitmap[i] &= ~(1 << (j+k));
            }

            return (i*WORD_BITS+j)*4096;
        }
    }

    return 0;
}

void memory_free(unsigned frame) {
    unsigned f = frame/4096;
    bitmap[f/WORD_BITS] |= 1 << (f % WORD_BITS);
}
