use core::mem::size_of;
use arch::paging;
use arch;

struct FrameStack {
    top: *mut usize,
    pos: usize
}

impl FrameStack {
    fn max_pos(&self) -> usize {
        arch::PAGE_SIZE/size_of::<usize>()
    }

    unsafe fn get(&self, index: usize) -> usize {
        return *self.top.offset(index as isize);
    }

    unsafe fn set(&mut self, index: usize, value: usize) {
        *self.top.offset(index as isize) = value;
    }

    pub unsafe fn push(&mut self, addr: usize) {
        // check if the current frame does not exist or if it is full
        if self.pos % self.max_pos() == 0 {
            // get the phyiscal address of the current frame or set it to 0
            let last = if self.top.is_null() { 0 } else { self.get(0) };

            // unmap the old top and map the provided addr as new top
            if !self.top.is_null() { paging::kernel_unmap(self.top) }
            self.top = paging::kernel_map(addr as *mut usize);

            // save the physical address of the current frame at position 0
            // and the pyhsical address of the last frame at position 1
            self.set(0, addr);
            self.set(1, last);

            // start filling the stack from position 2
            self.pos = 2;
        }
        else {
            let curr = self.pos;
            self.set(curr, addr);
            self.pos += 1;
        }
    }

    pub unsafe fn pop(&mut self) -> usize {
        if self.pos < 2 {
            return 0;
        }
        else if self.pos == 2 {
            let curr = self.get(0);
            let last = self.get(1);

            paging::kernel_unmap(self.top);

            if last == 0 {
                self.top = 0 as *mut usize;
                self.pos = 0;
            } else {
                self.top = paging::kernel_map(last as *mut usize);
                self.pos = self.max_pos();
            }

            return curr;
        }
        else {
            self.pos -= 1;
            return self.get(self.pos);
        }
    }
}

static mut stack: FrameStack = FrameStack { top: 0 as *mut usize, pos: 0 };

pub fn add_memory(base: usize, length: usize) {
    let mut start = base & arch::PAGE_MASK;
    let mut length = length;

    // align to page size
    if start < base || base == 0 {
        start += arch::PAGE_SIZE;
        length -= start - base;
    }

    let pages = length / arch::PAGE_SIZE;

    for i in 0..pages {
        let addr = start + i*arch::PAGE_SIZE;
        free(addr as *mut usize);
    }
}

pub fn alloc<T>() -> *mut T {
    let addr = unsafe { stack.pop() };
    if addr == 0 { panic!("out of memory"); }
    addr as *mut T
}

pub fn free<T>(ptr: *mut T) {
    unsafe { stack.push(ptr as usize) }
}
