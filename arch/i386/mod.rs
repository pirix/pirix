pub mod cpu;
pub mod io;
pub mod irq;
pub mod debug;
pub mod paging;
pub mod multiboot;

pub static PAGE_SIZE: usize = 0x1000;
pub static PAGE_MASK: usize = 0xfffff000;

pub fn init() {
    multiboot::init();
}
