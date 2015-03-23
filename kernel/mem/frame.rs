use core::prelude::*;
use core::mem::size_of;
use core::intrinsics::offset;
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

    fn get(&self, index: usize) -> usize {
        let top = self.top as *const usize;
        unsafe {
            let field = offset(top, index as isize);
            return *field;
        }
    }

    fn set(&mut self, index: usize, value: usize) {
        let top = self.top as *const usize;
        unsafe {
            let field = offset(top, index as isize) as *mut usize;
            *field = value;
        }
    }

    pub fn push(&mut self, addr: usize) {
        if self.pos % self.max_pos() == 0 {
            let last = self.top as usize;
            self.top = addr as *mut usize;
            self.set(0, last);
            self.pos = 1;
        }
        else {
            let curr = self.pos;
            self.set(curr, addr);
            self.pos += 1;
        }
    }

    pub fn pop(&mut self) -> usize {
        if self.top as usize == 0 {
            return 0;
        }
        else if self.pos as usize == 1 {
            let addr = self.top as usize;
            self.top = self.get(0) as *mut usize;
            self.pos = self.max_pos() - 1;
            return addr;
        }
        else {
            self.pos -= 1;
            let addr = self.get(self.pos);
            return addr;
        }
    }
}

static mut stack: FrameStack = FrameStack { top: 0 as *mut usize, pos: 0 };

pub fn add_memory(base: usize, length: usize) {
    let mut start = base & 0xfffff000;
    let mut length = length;

    if start < base {
        start += 0x1000;
        length -= start - base;
    }

    let pages = length / 0x1000;

    for i in (0..pages) {
        let addr = start + i*0x1000;
        free(addr as *mut usize);
    }
}

pub fn alloc<T>() -> *mut T {
    unsafe {
        let addr = stack.pop();
        if addr == 0 {
            panic!("out of memory");
        }
        addr as *mut T
    }
}

pub fn free<T>(ptr: *mut T) {
    unsafe { stack.push(ptr as usize) }
}
