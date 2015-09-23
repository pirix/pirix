use arch::cpu;
use process::ProcessRef;

pub type ThreadId = u16;

pub struct Thread {
    tid: ThreadId,
    process: ProcessRef,
    state: cpu::State
}
