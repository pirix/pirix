pub use arch::paging;
pub mod frame;
pub mod heap;
pub mod alloc;

pub fn init() {
    unsafe { paging::init(); }
}
