use spin::Mutex;

#[derive(Clone, Copy, Debug)]
struct Zone {
    start: usize,
    end: usize
}

impl Zone {
    pub fn alloc(&mut self, size: usize, align: usize) -> Option<usize> {
        let mask = align - 1;
        let start = (self.start + mask) & !mask;
        let end = start + size;

        if end <= self.end {
            self.start = end;
            Some(start)
        }
        else {
            None
        }
    }
}

static ZONES: Mutex<[Option<Zone>; 20]> = Mutex::new([Option::None; 20]);

pub fn add(base: usize, length: usize) {
    let mut base = base;
    let mut length = length;

    // make sure that there is no 0 address
    if base == 0 {
        if length > 0 {
            base += 1;
            length -= 1;
        }
        else {
            return;
        }
    }

    let mut zones = ZONES.lock();
    for i in 0..zones.len() {
        if zones[i].is_none() {
            zones[i] = Some(Zone {
                start: base,
                end: base + length
            });
            break;
        }
    }
}

pub unsafe fn alloc(size: usize, align: usize) -> Option<usize> {
    for zone in ZONES.lock().iter_mut() {
        if let &mut Some(ref mut zone) = zone {
            if let Some(addr) = zone.alloc(size, align) {
                return Some(addr);
            }
        }
        else {
            break;
        }
    }
    None
}
