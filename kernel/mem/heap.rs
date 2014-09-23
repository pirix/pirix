use mem;
use arch;
use core::prelude::*;
use core::mem::size_of;

static sizes: &'static [uint] = &[4, 8, 12, 16, 24, 32, 48, 64, 96, 128,
                                  192, 256, 384, 512, 768, 1024, 1536,
                                  2048, 3072, 4092];

static mut buckets: [*mut Slab, ..20] = [0 as *mut Slab, ..20];

#[packed]
struct Slab {
    next: *mut Slab
}

unsafe fn find_bucket(size: uint) -> uint {
    let mut i = 0;
    while size > sizes[i] {
        i += 1;
    }
    return i;
}

unsafe fn new_bucket(size: uint) {
    let addr: *mut Slab = mem::frame::alloc();
    let slab = mem::paging::kernel_map(addr);
    let data = (slab as uint) as *mut Slab;
    let bucket = find_bucket(size);

    for i in range(0, arch::PAGE_SIZE / size) {
        let index = (size / size_of::<Slab>()) * i;
        let entry = data.offset(index as int);
        (*entry).next = buckets[bucket];
        buckets[bucket] = entry;
    }
}

#[lang="exchange_malloc"]
pub unsafe fn alloc(size: uint, align: uint) -> *mut u8 {
    let bucket = find_bucket(size);

    if buckets[bucket].is_null() {
        new_bucket(size);
    }

    let slab = buckets[bucket];
    buckets[bucket] = (*slab).next;

    println!("alloc {} in {}", slab, bucket);
    return slab as *mut u8;
}

#[lang="exchange_free"]
pub unsafe fn free(addr: *mut u8, size: uint, align: uint) {
    let bucket = find_bucket(size);
    let slab = addr as *mut Slab;
    (*slab).next = buckets[bucket];
    buckets[bucket] = slab;

    println!("free {} in {}", slab, bucket);
}
