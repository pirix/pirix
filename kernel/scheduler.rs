use collections::vec_deque::VecDeque;
use process::ProcessRef;

pub struct Scheduler {
    queue: VecDeque<ProcessRef>
}

impl Scheduler {
    
}

#[no_mangle]
pub unsafe fn scheduler_schedule() {

}
