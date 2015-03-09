use core::u32;
use mem;

#[repr(packed)]
#[allow(dead_code)]
struct MultibootInfo {
    flags: u32,
    mem_lower: u32,
    mem_upper: u32,
    boot_device: u32,
    cmdline: u32,
    mod_count: u32,
    mods: *const Module,
    elf_num: u32,
    elf_size: u32,
    elf_addr: u32,
    elf_shndx: u32,
    mmap_length: u32,
    mmap: *const MmapEntry,
    drives_length: u32,
    drives_addr: u32,
    config_table: u32,
    boot_loader_name: u32,
    apm_table: u32,
    vbe_control_info: u32,
    vbe_mode_info: u32,
    vbe_mode: u16,
    vbe_interface_seg: u16,
    vbe_interface_off: u16,
    vbe_interface_len: u16
}

#[repr(packed)]
#[allow(dead_code)]
struct MmapEntry {
    size: u32,
    base: u64,
    length: u64,
    mem_type: u32
}

#[repr(packed)]
#[allow(dead_code)]
struct Module {
    start: u32,
    end: u32,
    args: u32,
    pad: u32
}

extern {
    static mb_magic: u32;
    static mb_info: *const MultibootInfo;
}

unsafe fn read_mmap(mmap: *const MmapEntry, mmap_length: usize) {
    let mut entry: *const MmapEntry = mmap;
    let end = (mmap as usize) + mmap_length;

    while (entry as usize) < end {
        let mut usable = true;
        let base = (*entry).base;
        let mut length = (*entry).length;

        if base > u32::MAX as u64 {
            usable = false;
        }
        else if base + length > u32::MAX as u64 {
            length = u32::MAX as u64 - base;
        }

        if usable {
            mem::frame::add_memory(base as usize, length as usize);
        }

        entry = ((entry as u32) + (*entry).size + 4) as *const MmapEntry;
    }
}

pub fn init() {
    if mb_magic != 0x2badb002 {
        panic!("invalid multiboot magic");
    }

     unsafe {
        read_mmap((*mb_info).mmap, (*mb_info).mmap_length as usize);
    }
}
