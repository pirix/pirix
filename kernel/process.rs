use arch::paging::PagingContext;
use core::cell::RefCell;

pub type ProcessId = u16;

pub struct Process {
    pid: ProcessId,
    context: PagingContext
}

pub type ProcessRef = RefCell<Process>;

impl Process {
    pub fn new() -> ProcessRef {
        RefCell::new(Process {
            pid: 1,
            context: PagingContext::new(),
        })
    }

    pub fn exit(&self, retval: u8) {
        log!("exit process {} with status {}", self.pid, retval);
    }

    pub fn kill(pid: u16, sig: u8) {

    }
}
