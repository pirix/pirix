use core::mem::size_of;
use core::intrinsics::offset;
use arch;

struct FrameStack {
    top: *mut uint,
    pos: uint
}

impl FrameStack {
    fn max_pos(&self) -> uint {
        arch::PAGE_SIZE/size_of::<uint>()
    }

    fn get(&self, index: uint) -> uint {
        let top = self.top as *uint;
        unsafe {
            let field = offset(top, index as int);
            return *field;
        }
    }

    fn set(&mut self, index: uint, value: uint) {
        let top = self.top as *uint;
        unsafe {
            let field = offset(top, index as int) as *mut uint;
            *field = value;
        }
    }

    pub fn push(&mut self, addr: uint) {
        if self.pos % self.max_pos() == 0 {
            let last = self.top as uint;
            self.top = addr as *mut uint;
            self.set(0, last);
            self.pos = 1;
        }
        else {
            let curr = self.pos;
            self.set(curr, addr);
            self.pos += 1;
        }
    }

    pub fn pop(&mut self) -> uint {
        if self.top as uint == 0 {
            return 0;
        }
        else if self.pos as uint == 1 {
            let addr = self.top as uint;
            self.top = self.get(0) as *mut uint;
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

static mut stack: FrameStack = FrameStack { top: 0 as *mut uint, pos: 0 };

pub fn init() {
    let mut addr = 0x100000;

    while addr < 0x1000000 {
        unsafe { stack.push(addr); }
        addr += 0x1000;
    }
}

pub fn alloc<T>() -> *mut T {
    unsafe { stack.pop() as *mut T }
}

pub fn free<T>(ptr: *T) {
    unsafe { stack.push(ptr as uint) }
}
