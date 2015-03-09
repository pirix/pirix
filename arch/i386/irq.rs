use arch::io;

static PIC1: u16 = 0x20;
static PIC2: u16 = 0xA0;
static PIC_EOI: u8 = 0x20;
static IRQ_BASE: u8 = 0x20;

pub unsafe fn init() {
    // start initialization
    io::outb(PIC1, 0x11);
    io::outb(PIC2, 0x11);

    // set IRQ base numbers for each PIC
    io::outb(PIC1+1, IRQ_BASE);
    io::outb(PIC2+1, IRQ_BASE+8);

    // use IRQ number 2 to relay IRQs from the slave PIC
    io::outb(PIC1+1, 0x04);
    io::outb(PIC2+1, 0x02);

    // finish initialization
    io::outb(PIC1+1, 0x01);
    io::outb(PIC2+1, 0x01);

    // mask all IRQs
    io::outb(PIC1+1, 0xff);
    io::outb(PIC2+1, 0xff);
}

fn find_pic(irq: u8) -> (u16, u8) {
    if irq >= IRQ_BASE && irq < IRQ_BASE + 8 {
        (PIC1, IRQ_BASE)
    }
    else if irq >= IRQ_BASE + 8 && irq < IRQ_BASE + 16 {
        (PIC2, IRQ_BASE+8)
    }
    else {
        (0, 0)
    }
}

pub unsafe fn allow(irq: usize) {
    let (pic, base) = find_pic(irq as u8);

    if pic == 0 { return; }

    let mut mask: u8 = io::inb(pic+1);
    mask &= !(1u8 << irq - (base as usize));
    io::outb(pic+1, mask);
}

pub unsafe fn disallow(irq: usize) {
    let (pic, base) = find_pic(irq as u8);

    if pic == 0 { return; }

    let mut mask: u8 = io::inb(pic+1);
    mask |= 1u8 << irq - (base as usize);
    io::outb(pic+1, mask);
}

#[inline]
pub unsafe fn start() {
    asm!("sti");
}

#[inline]
pub unsafe fn stop() {
    asm!("cli");
}

pub unsafe fn clear(irq: usize) {
    let (pic, _) = find_pic(irq as u8);

    if pic != 0 {
        io::outb(pic, PIC_EOI);
    }
}
