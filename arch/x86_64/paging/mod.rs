use mem::{Page, Frame};

pub mod entry;
mod table;

pub use self::entry::Entry;
use self::table::{Table, P4, Level4};

type Context = *mut Table<Level4>;

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

pub fn init() {

}

pub fn map(page: Page, frame: Frame, flags: entry::Flags) {
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

fn activate_context(context: Context) {
    unsafe { asm!("mov $0, %cr3" :: "r"(context)); }
}

fn invlpg(page: Page) {
    unsafe { asm!("invlpg [$0]" :: "r"(page.addr) : "memory" : "volatile", "intel"); }
}
