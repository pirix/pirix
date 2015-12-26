#[repr(packed)]
#[allow(dead_code)]
#[derive(Debug)]
pub struct State {
    r11: u64, r10: u64, r9: u64, r8: u64,
    rdi: u64, rsi: u64, rdx: u64, rcx: u64, rax: u64,
    pub irq: u64, pub err: u64
}

impl State {
    fn init(&mut self, entry: u32) {

    }

    fn set_kernel_mode(&mut self) {

    }

    fn set_stack(&mut self, stack: u32) {

    }
}
