use mem::frame;
use core::prelude::*;
use core::iter::range_step;
use core::mem::size_of;
use core::ptr::{zero_memory, copy_memory};

#[repr(packed)] pub struct PageTable { entries: [usize; 1024] }
#[repr(packed)] pub struct PageDir { entries: [usize; 1024] }
#[repr(packed)] pub struct PageTableDir { entries: [usize; 1024*1024] }

unsafe impl Sync for *mut PageDir {}
unsafe impl Sync for *mut PageTableDir {}

static page_dir: *mut PageDir = 0xffbff000us as *mut PageDir;
static page_tables: *mut PageTableDir = 0xffc00000us as *mut PageTableDir;

extern {
    static mut boot_page_dir: PageDir;
    static mut kernel_page_tables: [PageTable; 256];
}

impl PageDir {
    pub unsafe fn new() -> *mut PageDir {
        let dir: *mut PageDir = frame::alloc();
        let vdir = kernel_map(dir);

        let table: *mut PageTable = frame::alloc();
        let vtable = kernel_map(table);

        zero_memory(vtable, 1024);
        zero_memory(vdir, 768);

        let kernel_tables = page_dir.offset(768) as *const PageDir;
        copy_memory(vdir.offset(768), kernel_tables, 256);

        (*vtable).set(1023, dir as usize, 0x3);
        (*vdir).set(1022, table, 0x3);
        (*vdir).set(1023, dir as *mut PageTable, 0x3);

        kernel_unmap(vtable);
        kernel_unmap(vdir);

        return dir;
    }

    pub fn set(&mut self, index: usize, table: *mut PageTable, flags: usize) {
        self.entries[index] = table as usize | flags;
    }

    pub fn get(&self, index: usize) -> usize {
        self.entries[index]
    }

    pub fn get_table(&self, index: usize) -> *mut PageTable {
        (self.entries[index] & 0xfffff000) as *mut PageTable
    }
}

impl PageTable {
    pub fn new() -> *mut PageTable {
        return frame::alloc();
    }

    pub unsafe fn at(index: usize) -> *mut PageTable {
        page_tables.offset((index*1024) as isize) as *mut PageTable
    }

    pub unsafe fn clear(&mut self) {
        zero_memory(self.entries.as_mut_ptr(), 1024);
    }

    pub fn set(&mut self, index: usize, page: usize, flags: usize) {
        self.entries[index] = page | flags;
    }

    pub fn get(&self, index: usize) -> usize {
        self.entries[index]
    }

    pub fn get_page(&self, index: usize) -> usize {
        self.entries[index] & 0xfffff000
    }
}

pub unsafe fn init() {
    let physdir: *mut PageDir = &mut boot_page_dir;
    let dir = ((physdir as usize) + 0xc0000000) as *mut PageDir;

    // create kernel pages
    for i in range(768, 1023us) {
        let table = kernel_page_tables.get_mut(i-768).unwrap() as *mut PageTable;

        loop {}

        (*table).clear();

        if i == 768 {
            for j in range(0, 1024us) {
                (*table).set(j, j*0x1000, 0x103);
            }
        }
        else if i == 1022 {
            (*table).set(1023, dir as usize, 0x3);
        }

        log!("Holding");
        loop {}

        let phystable = (table as usize - 0xc0000000) as *mut PageTable;
        (*dir).set(i, phystable, 0x3);
    }

    (*dir).set(1023, physdir as *mut PageTable, 0x3);

    activate_pagedir(physdir);
}

pub unsafe fn map(virt: usize, phys: usize, flags: usize) {
    let pdidx = virt >> 22;
    let ptidx = (virt >> 12) & 0x3ff;

    if (*page_dir).get(pdidx) & 0x1 == 0 {
        let table = PageTable::new();
        (*page_dir).set(pdidx, table, flags | 0x3);
        let vtable = PageTable::at(pdidx);
        invlpg(vtable as usize);
        (*vtable).clear();
    }

    let table = PageTable::at(pdidx);
    (*table).set(ptidx, phys, flags | 0x3);

    invlpg(virt);
}

pub unsafe fn unmap(virt: usize) {
    let pdidx = virt >> 22;
    let ptidx = (virt >> 12) & 0x3ff;

    let table = PageTable::at(pdidx);
    (*table).set(ptidx, 0, 0);

    invlpg(virt);
}

pub unsafe fn getphys<T>(virt: *mut T) -> *mut T {
    let virt = virt as usize;

    let pdidx = virt >> 22;
    let ptidx = (virt >> 12) & 0x3ff;

    let table = PageTable::at(pdidx);
    let entry = (*table).get(ptidx);

    return ((entry & 0xfffff000) | (virt & 0xfff)) as *mut T;
}

fn calculate_page_count<T>(addr: *mut T) -> usize {
    let size = size_of::<T>();
    let mut page_count = (size+0x1000-1) / 0x1000;

    if (addr as usize % 0x1000) + size > 0x1000 {
        page_count += 1;
    }

    return page_count;
}

unsafe fn find_mapping_area(page_count: usize) -> usize {
    let mut continous_start = 0;
    let mut continous_count = 0;

    for virt in range_step(0xd0000000, 0xe0000000, 0x1000us) {
        let table = PageTable::at(virt >> 22);
        let index = virt >> 12 & 0x3ff;

        if (*table).get(index) & 0x1 == 0x1 {
            continous_start = 0;
            continue;
        }

        if continous_start == 0 {
            continous_start = virt;
            continous_count = 1;
        }
        else {
            continous_count += 1;
        }

        if continous_count >= page_count {
            return continous_start;
        }
    }

    return 0;
}

pub unsafe fn kernel_map<T>(addr: *mut T) -> *mut T {
    let page_count = calculate_page_count(addr);
    let mapping_area = find_mapping_area(page_count);

    if mapping_area == 0 {
        panic!("kernel mapping area full");
    }

    for i in range(0, page_count) {
        let virt = mapping_area + i*0x1000;
        map(virt, addr as usize + i*0x1000, 0x103);
    }

    let vaddr = mapping_area + (addr as usize & 0xfff);

    return vaddr as *mut T;
}

pub unsafe fn kernel_unmap<T>(addr: *mut T) {
    let page_count = calculate_page_count(addr);

    for i in range(0, page_count) {
        let virt = (addr as usize) + i*0x1000;
        unmap(virt);
    }
}

pub unsafe fn activate_pagedir(dir: *mut PageDir) {
    asm!("mov $0, %cr3" :: "r"(dir));
}

unsafe fn invlpg(addr: usize) {
    asm!("invlpg [$0]" :: "r"(addr) : "memory" : "volatile", "intel");
}
