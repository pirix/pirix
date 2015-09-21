pub use arch::paging;
pub mod frame;
pub mod heap;
pub mod boxed;

pub fn init() {
    unsafe { paging::init(); }
}
