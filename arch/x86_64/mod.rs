pub mod io;
pub mod cpu;
pub mod irq;
pub mod debug;
pub mod paging;
pub mod multiboot;

pub const PAGE_SIZE: usize = 0x1000;
pub const PAGE_MASK: usize = 0xfffffffffffff000;
pub const ADDR_SIZE: usize = 0x8;
pub const IRQ_COUNT: usize = 256;

pub fn init() {
    multiboot::init();
}