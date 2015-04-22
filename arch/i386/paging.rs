use mem::frame;
use core::prelude::*;
use core::mem::size_of;
use core::ptr::{write_bytes, copy};

#[repr(packed)] pub struct PageTable { entries: [usize; 1024] }
#[repr(packed)] pub struct PageDir { entries: [usize; 1024] }

extern {
    static mut boot_page_dir: PageDir;
    static mut kernel_page_tables: [PageTable; 256];
}

pub struct MappedTables {
    page_dir: *mut PageDir,
    page_tables: *mut PageTable
}

unsafe impl Sync for MappedTables {}

static mapped: MappedTables = MappedTables {
    page_dir: 0xfffff000 as *mut PageDir,
    page_tables: 0xffc00000 as *mut PageTable
};

impl PageDir {
    pub unsafe fn new() -> *mut PageDir {
        let dir: *mut PageDir = frame::alloc();
        let vdir = kernel_map(dir);

        let table: *mut PageTable = frame::alloc();
        let vtable = kernel_map(table);

        write_bytes(vtable, 0, 1024);
        write_bytes(vdir, 0, 768);

        let kernel_tables = mapped.page_dir.offset(768) as *const PageDir;
        copy(vdir.offset(768), kernel_tables, 256);

        (*vtable).set(1023, dir as usize, 0x3);
        (*vdir).set(1022, table, 0x3);
        (*vdir).set(1023, dir as *mut PageTable, 0x3);

        kernel_unmap(vtable);
        kernel_unmap(vdir);

        return dir;
    }

    pub unsafe fn mapped() -> *mut PageDir {
        mapped.page_dir as *mut PageDir
    }

    pub fn set(&mut self, index: usize, table: *mut PageTable, flags: usize) {
        self.entries[index] = table as usize | flags;
    }

    pub fn get(&self, index: usize) -> usize {
        self.entries[index]
    }
}

impl PageTable {
    pub fn new() -> *mut PageTable {
        return frame::alloc();
    }

    pub unsafe fn at(index: usize) -> *mut PageTable {
        mapped.page_tables.offset(index as isize) as *mut PageTable
    }

    pub unsafe fn clear(&mut self) {
        write_bytes(self.entries.as_mut_ptr(), 0, 1024);
    }

    pub fn set(&mut self, index: usize, page: usize, flags: usize) {
        self.entries[index] = page | flags;
    }

    pub fn get(&self, index: usize) -> usize {
        self.entries[index]
    }
}

pub unsafe fn init() {
    let physdir: *mut PageDir = &mut boot_page_dir;
    let dir = ((physdir as usize) + 0xc0000000) as *mut PageDir;

    // create kernel pages
    for i in (768..1023) {
        let table = kernel_page_tables.get_mut(i-768).unwrap() as *mut PageTable;

        (*table).clear();

        if i == 768 {
            for j in (0..1024) {
                (*table).set(j, j*0x1000, 0x103);
            }
        }
        else if i == 1022 {
            (*table).set(1023, dir as usize, 0x3);
        }

        let phystable = (table as usize - 0xc0000000) as *mut PageTable;
        (*dir).set(i, phystable, 0x3);
    }

    (*dir).set(1023, physdir as *mut PageTable, 0x3);

    activate_pagedir(physdir);
}

pub unsafe fn map(virt: usize, phys: usize, flags: usize) {
    let dir = PageDir::mapped();
    let pdidx = virt >> 22;
    let ptidx = (virt >> 12) & 0x3ff;

    if (*dir).get(pdidx) & 0x1 == 0 {
        let table = PageTable::new();
        (*dir).set(pdidx, table, flags | 0x3);
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

    for virt in (0xd0000000..0xe0000000).step_by(0x1000) {
        let table = PageTable::at(virt >> 22);
        let index = (virt >> 12) & 0x3ff;

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

    for i in (0..page_count) {
        let virt = mapping_area + i*0x1000;
        map(virt, addr as usize + i*0x1000, 0x103);
    }

    let vaddr = mapping_area + (addr as usize & 0xfff);

    return vaddr as *mut T;
}

pub unsafe fn kernel_unmap<T>(addr: *mut T) {
    let page_count = calculate_page_count(addr);

    for i in (0..page_count) {
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
