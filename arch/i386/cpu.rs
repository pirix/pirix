#[repr(packed)]
#[allow(dead_code)]
pub struct State {
    gs: u32, ds: u32,
    edi: u32, esi: u32, ebp: u32, esp: u32,
    ebx: u32, edx: u32, ecx: u32, eax: u32,
    pub irq: u32, pub err: u32,
    eip: u32, cs: u32, eflags: u32, usr_esp: u32, ss: u32
}

impl State {
    fn init(&mut self, entry: u32) {
        self.eip = entry;
        self.cs = 0x18 | 0x3;
        self.ss = 0x20 | 0x3;
        self.ds = 0x20 | 0x3;
        self.gs = 0x20 | 0x3;
        self.eflags = 0x202;
    }

    fn set_kernel_mode(&mut self) {
        self.cs = 0x08 | 0x3;
        self.ss = 0x10 | 0x3;
        self.ds = 0x10 | 0x3;
        self.gs = 0x10 | 0x3;
        self.eflags = 0x202;
    }

    fn set_stack(&mut self, stack: u32) {
        self.usr_esp = stack;
    }
}
