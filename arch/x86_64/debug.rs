mod video {
    use arch::io;

    static VMEM: usize = 0xffffff78000b8000;
    static mut pos: (usize, usize) = (0, 0);

    pub unsafe fn clear() {
        for i in 0..80*25 {
            *((VMEM + i*2) as *mut u16) = 0;
        }
    }

    pub unsafe fn init() {
        clear();
    }

    unsafe fn scroll() {
        for i in 0..80*24 {
            *((VMEM + i*2) as *mut u16) = *((VMEM + (i+80)*2) as *mut u16);
        }
        for i in 80*24..80*25 {
            *((VMEM + i*2) as *mut u16) = 0;
        }
    }

    unsafe fn cursor() {
        let (x, y) = pos;
        let index = y*80 + x;

        io::outb(0x3d4, 14);
        io::outb(0x3d5, (index >> 8) as u8);
        io::outb(0x3d4, 15);
        io::outb(0x3d5, index as u8);
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
                let addr = (VMEM + (x + y*80)*2) as *mut u16;
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
