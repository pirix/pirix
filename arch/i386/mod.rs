pub mod cpu;
pub mod io;
pub mod irq;
pub mod debug;
pub mod paging;
pub mod multiboot;

pub const PAGE_SIZE: usize = 0x1000;
pub const PAGE_MASK: usize = 0xfffff000;
pub const IRQ_COUNT: usize = 256;

pub fn init() {
    multiboot::init();
}
