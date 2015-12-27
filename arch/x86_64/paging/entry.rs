use mem::Frame;

bitflags! {
    flags Flags: u64 {
        const PRESENT =         1 << 0,
        const WRITABLE =        1 << 1,
        const USER =            1 << 2,
        const WRITE_THROUGH =   1 << 3,
        const NO_CACHE =        1 << 4,
        const ACCESSED =        1 << 5,
        const DIRTY =           1 << 6,
        const HUGE =            1 << 7,
        const GLOBAL =          1 << 8,
        const NO_EXECUTE =      1 << 63,
    }
}

#[derive(Clone, Copy)]
pub struct Entry(u64);

impl Entry {
    pub fn new(frame: Frame, flags: Flags) -> Entry {
        let mut entry = Entry(frame.addr as u64);
        entry.set_flags(flags | PRESENT);
        entry
    }

    pub fn set_flags(&mut self, flags: Flags) {
        self.0 |= flags.bits();
    }

    pub fn flags(&self) -> Flags {
        Flags::from_bits_truncate(self.0)
    }

    pub fn is_unused(&self) -> bool {
        self.0 == 0
    }

    pub fn set_unused(&mut self) {
        self.0 = 0;
    }

    pub fn is_present(&self) -> bool {
        self.flags().contains(PRESENT)
    }

    pub fn is_huge(&self) -> bool {
        self.flags().contains(HUGE)
    }
}
