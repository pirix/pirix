use arch::paging::PagingContext;

pub type ProcessId = u16;

pub struct Process {
    pid: ProcessId,
    context: PagingContext
}

impl Process {
    pub fn new() -> Process {
        Process {
            pid: 1,
            context: PagingContext::new(),
        }
    }

    pub fn exit(&self, retval: u8) {

    }

    pub fn kill(pid: u16, sig: u8) {

    }
}
