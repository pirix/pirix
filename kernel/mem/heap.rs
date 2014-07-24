use mem;
use core::mem::size_of;
use core::intrinsics::offset;

static BUCKET_COUNT: uint = 16;
static mut buckets: [*mut Block, ..BUCKET_COUNT] = [0 as *mut Block, ..BUCKET_COUNT];

struct Block {
    next: *mut Block
}

fn find_bucket(size: uint) -> uint {
    let mut bucket = BUCKET_COUNT;
    let mut i = 0;

    while i < BUCKET_COUNT {
        if size <= 4*(1 << i) {
            bucket = i;
            break;
        }
        i += 1;
    }

    return bucket;
}

unsafe fn create_slab(bucket: uint) -> *mut Block {
    let addr: uint = mem::frame::alloc();
    let addrsize: uint = size_of::<uint>();
    let slab: uint = addr; // mem::paging::map_kernel(addr, 0x1000);
    let mut i: uint = 0;

    while i < 0x1000/addrsize {
        let entry: *uint = offset(slab as *uint, i as int);
        free(entry, addrsize*(1 << bucket));
        i += 1 << bucket;
    }

    return addr as *mut Block;
}

pub unsafe fn alloc(size: uint) -> *uint {
    let bucket = find_bucket(size);

    if buckets[bucket] as int == 0 {
        buckets[bucket] = create_slab(bucket);
    }

    let block = buckets[bucket];
    buckets[bucket] = (*block).next;
    return block as *uint;
}

pub unsafe fn free(addr: *uint, size: uint) {
    let bucket = find_bucket(size);
    let block = addr as *mut Block;
    (*block).next = buckets[bucket];
    buckets[bucket] = block;
}
