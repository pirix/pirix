pub use arch::paging;
pub mod zone;
pub mod frame;
pub mod heap;
pub mod alloc;

pub use self::frame::Frame;

pub struct Page {
   pub addr: usize
}

pub fn init() {
    unsafe { paging::init(); }
}
