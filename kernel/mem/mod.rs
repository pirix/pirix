pub use arch::paging;
pub mod frame;
pub mod heap;

pub fn init() {
    unsafe { paging::init(); }
}
