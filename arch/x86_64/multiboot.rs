use core::cmp::{max, min};
use mem;

#[repr(C, packed)]
struct MultibootInfo {
    pub total_size: u32,
    _reserved: u32,
    first_tag: Tag
}

impl MultibootInfo {
    pub fn memory_map_tag(&self) -> Option<&'static MemoryMapTag> {
        self.get_tag(6).map(|tag| unsafe{ &*(tag as *const Tag as *const MemoryMapTag)} )
    }

    fn has_valid_end_tag(&self) -> bool {
        let self_ptr = self as *const _;
        let end_tag_addr = self_ptr as usize + (self.total_size - 8) as usize;
        let end_tag = unsafe { &*(end_tag_addr as *const Tag) };
        end_tag.tag_type == 0 && end_tag.size == 8
    }

    fn get_tag(&self, tag_type: u32) -> Option<&'static Tag> {
        self.tags().find(|tag| tag.tag_type == tag_type)
    }

    fn tags(&self) -> TagIter {
        TagIter { current: &self.first_tag as *const _ }
    }
}

#[derive(Debug)]
#[repr(C, packed)]
struct Tag {
    tag_type: u32,
    size: u32
}

struct TagIter {
    current: *const Tag,
}

impl Iterator for TagIter {
    type Item = &'static Tag;

    fn next(&mut self) -> Option<&'static Tag> {
        match unsafe { &*self.current } {
            &Tag { tag_type: 0, size: 8 } => None,
            tag => {
                let mut tag_addr = self.current as usize;
                tag_addr += tag.size as usize;
                tag_addr = ((tag_addr-1) & !0x7) + 0x8; //align at 8 byte
                self.current = tag_addr as *const _;

                Some(tag)
            },
        }
    }
}

#[repr(C)]
pub struct MemoryMapTag {
    tag_type: u32,
    size: u32,
    entry_size: u32,
    entry_version: u32,
    first_area: MemoryArea
}

impl MemoryMapTag {
    pub fn memory_areas(&self) -> MemoryAreaIter {
        let self_ptr = self as *const MemoryMapTag;
        let start_area = (&self.first_area) as *const MemoryArea;
        MemoryAreaIter {
            current_area: start_area,
            last_area: ((self_ptr as u32) + self.size - self.entry_size) as *const MemoryArea,
            entry_size: self.entry_size,
        }
    }
}

#[derive(Debug)]
#[repr(C)]
pub struct MemoryArea {
    pub base_addr: u64,
    pub length: u64,
    area_type: u32,
    _reserved: u32
}

impl MemoryArea {
    pub fn usable(&self) -> bool {
        self.area_type == 1
    }
}

#[derive(Clone)]
pub struct MemoryAreaIter {
    current_area: *const MemoryArea,
    last_area: *const MemoryArea,
    entry_size: u32
}

impl Iterator for MemoryAreaIter {
    type Item = &'static MemoryArea;
    fn next(&mut self) -> Option<&'static MemoryArea> {
        if self.current_area > self.last_area { return None; }
        let area = unsafe { &*self.current_area };
        self.current_area = ((self.current_area as u32) + self.entry_size) as *const MemoryArea;
        if area.area_type == 1 { Some(area) } else { self.next() }
    }
}

extern "C" {
    static _multiboot_magic: u64;
    static _multiboot_info: &'static MultibootInfo;
    static _kernel_start: u64;
    static _kernel_end: u64;
}

fn read_memory_map(memory_map: &MemoryMapTag) {
    for area in memory_map.memory_areas() {
        if !area.usable() { continue; }
        let start = area.base_addr;
        let end = start + area.length;

        if max(start, _kernel_start) <= min(end, _kernel_end) {
            if start < _kernel_start {
                mem::frame::add_memory(start as usize, (_kernel_start - start) as usize);
            }
            if end > _kernel_end {
                mem::frame::add_memory(_kernel_end as usize, (end - _kernel_end) as usize);
            }
        }
        else {
            mem::frame::add_memory(area.base_addr as usize, area.length as usize);
        }
    }
}

pub fn init() {
    if _multiboot_magic != 0x36d76289 {
        panic!("invalid multiboot magic");
    }

    if !_multiboot_info.has_valid_end_tag() {
        panic!("invalid multiboot info");
    }

    if let Some(memory_map) = _multiboot_info.memory_map_tag() {
        read_memory_map(memory_map);
    }
}
