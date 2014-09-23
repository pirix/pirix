use core::prelude::*;
use core::u32;
use mem;

#[packed]
#[allow(dead_code)]
struct MultibootInfo {
    flags: u32,
    mem_lower: u32,
    mem_upper: u32,
    boot_device: u32,
    cmdline: u32,
    mod_count: u32,
    mods: *Module,
    elf_num: u32,
    elf_size: u32,
    elf_addr: u32,
    elf_shndx: u32,
    mmap_length: u32,
    mmap: *MmapEntry,
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

#[packed]
#[allow(dead_code)]
struct MmapEntry {
    size: u32,
    base: u64,
    length: u64,
    mem_type: u32
}

#[packed]
#[allow(dead_code)]
struct Module {
    start: u32,
    end: u32,
    args: u32,
    pad: u32
}

extern {
    static mb_magic: u32;
    static mb_info: *MultibootInfo;
}

unsafe fn read_mmap(mmap: *MmapEntry, mmap_length: uint) {
    let mut entry: *MmapEntry = mmap;
    let end = (mmap as uint) + mmap_length;

    while (entry as uint) < end {
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
            mem::frame::add_memory(base as uint, length as uint);
        }

        entry = ((entry as u32) + (*entry).size + 4) as *MmapEntry;
    }
}

pub fn init() {
    if mb_magic != 0x2badb002 {
        fail!("invalid multiboot magic");
    }

    let info = unsafe { *mb_info };

    unsafe {
        read_mmap(info.mmap, info.mmap_length as uint);
    }
}
