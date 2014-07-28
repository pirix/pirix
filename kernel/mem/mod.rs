pub use arch::paging;
pub mod frame;
pub mod heap;

pub fn init() {
    frame::init();
    unsafe { paging::init(); }
}
