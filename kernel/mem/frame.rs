use core::mem::size_of;
use mem::zone;
use spin::Mutex;
use arch::paging;
use arch;

pub struct Frame {
    pub addr: usize
}

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
            // get the physical address of the current frame or set it to 0
            let last = if self.top.is_null() { 0 } else { self.get(0) };

            // unmap the old top and map the provided addr as new top
            if !self.top.is_null() { paging::kernel_unmap(self.top) }
            self.top = paging::kernel_map(addr as *mut usize);

            // save the physical address of the current frame at position 0
            // and the physical address of the last frame at position 1
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

static STACK: Mutex<FrameStack> = Mutex::new(FrameStack { top: 0 as *mut usize, pos: 0 });

pub fn alloc() -> Frame {
    let mut addr = unsafe { STACK.lock().pop() };
    if addr == 0 {
        if let Some(frame_addr) = unsafe { zone::alloc(arch::PAGE_SIZE, arch::PAGE_SIZE) } {
            addr = frame_addr;
        }
        else { panic!("out of memory"); }
    }
    Frame { addr: addr }
}

pub fn free(frame: Frame) {
    unsafe { STACK.lock().push(frame.addr) }
}
