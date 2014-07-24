use mem::frame;
use core::mem;
use core::intrinsics::set_memory;

pub type PageDir = [u32, ..1024];
pub type PageTable = [u32, ..1024];

//static page_dir : PageDir = transmute(0xffbff000u);
//static page_tables : [PageTable, ..1024] = transmute(0xffc00000u);

extern {
    static boot_page_dir : [u32, ..1024];
}

pub unsafe fn init() {
    let mut dir: PageDir = boot_page_dir;
    let mut i = 0;

    while i < 255 {
        let addr = frame::alloc();
        let mut table: PageTable = mem::transmute(addr);
        dir[i+768] = addr as u32 | 0x3;

        set_memory(addr as *mut uint, 0, 0x1000);

        i += 1;

        if (i == 0) {
            let mut j = 0;
            while j < 1024 {
                table[j] = (j as u32) * 0x1000 | 0x103;
                j += 2;
            }
        }
        else if (i == 254) {
            let dir_addr: u32 = mem::transmute(&dir);
            table[1023] = dir_addr as u32 | 0x3;
        }
    }
}

pub unsafe fn map(virt: uint, phys: uint, flags: uint) {

}

pub unsafe fn activate_pagedir(dir: PageDir) {
    asm!("mov $0, %cr3" :: "r"(dir));
}
