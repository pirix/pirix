mod video {
    use arch::io;
    use core::prelude::*;

    static vmem : uint = 0xc00b8000;
    static mut pos : (uint, uint) = (0, 0);

    pub unsafe fn clear() {
        for i in range(0u, 80*25) {
            *((vmem + i*2) as *mut u16) = 0;
        }
    }

    pub unsafe fn init() {
        clear();
    }

    unsafe fn scroll() {
        for i in range(0u, 80u*24u) {
            *((vmem + i*2) as *mut u16) = *((vmem + (i+80)*2) as *mut u16);
        }
        for i in range(80u*24u, 80u*25u) {
            *((vmem + i*2) as *mut u16) = 0;
        }
    }

    unsafe fn cursor() {
        let (x, y) = pos;
        let pos = y*80 + x;

        io::outb(0x3d4, 14);
        io::outb(0x3d5, (pos >> 8) as u8);
        io::outb(0x3d4, 15);
        io::outb(0x3d5, pos as u8);
    }

    pub unsafe fn putchar(c: char) {
        let (mut x, mut y) = pos;

        match c {
            '\n' => {
                x = 0;
                y += 1;
            },
            '\r' => {
                x = 0;
            },
            _ => {
                let addr = (vmem + (x + y*80)*2) as *mut u16;
                *addr = 0x0f << 8 | (c as u16);
                x += 1;
            }
        }

        if x >= 80 { x = 0; y += 1; }
        if y >= 25 { scroll(); x = 0; y = 24 }

        pos = (x, y);
        cursor();
    }
}

mod serial {
    use arch::io;

    static PORT: u16 = 0x3f8;

    pub unsafe fn init() {
        io::outb(PORT+1, 0x00);
        io::outb(PORT+3, 0x80);
        io::outb(PORT+0, 0x03);
        io::outb(PORT+1, 0x00);
        io::outb(PORT+3, 0x03);
        io::outb(PORT+2, 0xC7);
        io::outb(PORT+4, 0x0B);
    }

    pub unsafe fn putchar(c: char) {
        while (io::inb(PORT+5) & 0x20) == 0 { }
        io::outb(PORT, c as u8);
    }
}

pub fn init() {
    unsafe {
        serial::init();
        video::init();
    }
}

pub fn putchar(c: char) {
    unsafe {
        serial::putchar(c);
        video::putchar(c);
    }
}
