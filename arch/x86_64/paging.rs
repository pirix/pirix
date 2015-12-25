use core::marker::PhantomData;
use mem;
use mem::{Page, Frame};

const PAGE_PRESENT: usize = 1 << 0;
const PAGE_WRITABLE: usize = 1 << 1;
const PAGE_USER: usize = 1 << 2;
const PAGE_HUGE: usize = 1 << 7;
const PAGE_GLOBAL: usize = 1 << 8;

impl Page {
    pub fn p4_index(&self) -> usize {
        (self.addr >> 27) & 0o777
    }

    pub fn p3_index(&self) -> usize {
        (self.addr >> 18) & 0o777
    }

    pub fn p2_index(&self) -> usize {
        (self.addr >> 9) & 0o777
    }

    pub fn p1_index(&self) -> usize {
        (self.addr >> 0) & 0o777
    }
}

#[derive(Clone, Copy)]
struct Entry(usize);

impl Entry {
    pub fn new(frame: Frame, flags: usize) -> Entry {
        let mut entry = Entry(frame.addr);
        entry.set_flags(flags | PAGE_PRESENT);
        entry
    }

    pub fn set_flags(&mut self, flags: usize) {
        self.0 |= flags & 0b110011111;
    }

    pub fn is_unused(&self) -> bool {
        self.0 == 0
    }

    pub fn set_unused(&mut self) {
        self.0 = 0;
    }

    pub fn is_present(&self) -> bool {
        self.0 & PAGE_PRESENT != 0
    }

    pub fn is_huge(&self) -> bool {
        self.0 & PAGE_HUGE != 0
    }
}

struct Table<L: TableLevel> {
    entries: [Entry; 512],
    level: PhantomData<L>
}

const P4: *mut Table<Level4> = 0xffffffff_fffff000 as *mut _;

impl<L> Table<L> where L: TableLevel {
    pub fn clear(&mut self) {
        for entry in self.entries.iter_mut() {
            entry.set_unused();
        }
    }

    pub fn get(&self, index: usize) -> Entry {
        self.entries[index]
    }

    pub fn set(&mut self, index: usize, entry: Entry) {
        self.entries[index] = entry;
    }
}

impl<L> Table<L> where L: HierarchicalLevel {
    fn next_table_address(&self, index: usize) -> Option<usize> {
        let entry = self.get(index);

        if entry.is_present() && !entry.is_huge() {
            let table_address = self as *const _ as usize;
            Some((table_address << 9) | (index << 12))
        }
        else {
            None
        }
    }

    pub fn next_table(&self, index: usize) -> Option<&Table<L::NextLevel>> {
        self.next_table_address(index)
            .map(|address| unsafe { &*(address as *const _) })
    }

    pub fn next_table_mut(&mut self, index: usize) -> Option<&mut Table<L::NextLevel>> {
        self.next_table_address(index)
            .map(|address| unsafe { &mut *(address as *mut _) })
    }

    pub fn next_table_create(&mut self, index: usize) -> &mut Table<L::NextLevel> {
        if self.next_table(index).is_none() {
            let frame = mem::frame::alloc();
            let entry = Entry::new(frame, PAGE_WRITABLE);
            self.set(index, entry);
            self.next_table_mut(index).unwrap().clear();
        }
        self.next_table_mut(index).unwrap()
    }
}

trait TableLevel {}

enum Level4 {}
enum Level3 {}
enum Level2 {}
enum Level1 {}

impl TableLevel for Level4 {}
impl TableLevel for Level3 {}
impl TableLevel for Level2 {}
impl TableLevel for Level1 {}

trait HierarchicalLevel: TableLevel {
    type NextLevel: TableLevel;
}

impl HierarchicalLevel for Level4 {
    type NextLevel = Level3;
}

impl HierarchicalLevel for Level3 {
    type NextLevel = Level2;
}

impl HierarchicalLevel for Level2 {
    type NextLevel = Level1;
}

pub fn init() {

}

pub fn map(page: Page, frame: Frame, flags: usize) {
    let p4 = unsafe { &mut *P4 };
    let mut p3 = p4.next_table_create(page.p4_index());
    let mut p2 = p3.next_table_create(page.p3_index());
    let mut p1 = p2.next_table_create(page.p2_index());

    p1.set(page.p1_index(), Entry::new(frame, flags));
    invlpg(page);
}

pub unsafe fn kernel_map<T>(addr: *mut T) -> *mut T {
    return 0 as *mut T;
}

pub unsafe fn kernel_unmap<T>(addr: *mut T) {

}

fn activate_p4(p4: *mut Table<Level4>) {
    unsafe { asm!("mov $0, %cr3" :: "r"(p4)); }
}

fn invlpg(page: Page) {
    unsafe { asm!("invlpg [$0]" :: "r"(page.addr) : "memory" : "volatile", "intel"); }
}
