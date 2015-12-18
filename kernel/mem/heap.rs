use mem;
use arch;
use core::mem::size_of;

static SIZES: &'static [usize] = &[4, 8, 12, 16, 24, 32, 48, 64, 96, 128,
                                  192, 256, 384, 512, 768, 1024, 1536,
                                  2048, 3072, 4092];

static mut buckets: [*mut Slab; 20] = [0 as *mut Slab; 20];

#[repr(packed)]
struct Slab {
    next: *mut Slab
}

unsafe fn find_bucket(size: usize) -> usize {
    let mut i = 0;
    while size > SIZES[i] {
        i += 1;
    }
    return i;
}

unsafe fn new_bucket(bucket: usize) {
    let addr: *mut Slab = mem::frame::alloc();
    let slab = mem::paging::kernel_map(addr);
    let data = (slab as usize) as *mut Slab;
    let size = SIZES[bucket];

    for i in 0..(arch::PAGE_SIZE / size) {
        let index = (size / size_of::<Slab>()) * i;
        let entry = data.offset(index as isize);
        (*entry).next = buckets[bucket];
        buckets[bucket] = entry;
    }
}

pub unsafe fn alloc(size: usize, align: usize) -> *mut u8 {
    let bucket = find_bucket(size);

    if buckets[bucket].is_null() {
        new_bucket(bucket);
    }

    let slab = buckets[bucket];
    buckets[bucket] = (*slab).next;

    log!("alloc {}: {:x} in {}", size, slab as usize, bucket);
    return slab as *mut u8;
}

pub unsafe fn free(addr: *mut u8, size: usize, align: usize) {
    let bucket = find_bucket(size);
    let slab = addr as *mut Slab;
    (*slab).next = buckets[bucket];
    buckets[bucket] = slab;

    log!("free {:p} in {}", slab, bucket);
}
