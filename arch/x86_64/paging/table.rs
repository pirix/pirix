use arch::paging::{entry, Entry};
use core::marker::PhantomData;
use mem;

pub struct Table<L: TableLevel> {
    entries: [Entry; 512],
    level: PhantomData<L>
}

pub const P4: *mut Table<Level4> = 0xffffffff_fffff000 as *mut _;

impl<L> Table<L> where  L: TableLevel {
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
            let entry = Entry::new(frame, entry::WRITABLE);
            self.set(index, entry);
            self.next_table_mut(index).unwrap().clear();
        }
        self.next_table_mut(index).unwrap()
    }
}

pub trait TableLevel {}

pub enum Level4 {}
pub enum Level3 {}
pub enum Level2 {}
pub enum Level1 {}

impl TableLevel for Level4 {}
impl TableLevel for Level3 {}
impl TableLevel for Level2 {}
impl TableLevel for Level1 {}

pub trait HierarchicalLevel: TableLevel {
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
