#define PL110_CR_EN   0x001
#define PL110_CR_PWR  0x800
#define PL110_IOBASE  0xc0000000
#define PL110_PALBASE (PL110_IOBASE + 0x200)

typedef struct pl110_mmio {
    unsigned tim0;
    unsigned tim1;
    unsigned tim2;
    unsigned d;
    unsigned upbase;
    unsigned f;
    unsigned g;
    unsigned control;
} pl110_mmio;

void video_init() {
    pl110_mmio* plio = (pl110_mmio*)PL110_IOBASE;

    plio->tim0 = 0x3f1f3f9c;
    plio->tim1 = 0x080b61df;
    plio->upbase = 0x200000;

    plio->control = 0x1829;
}
