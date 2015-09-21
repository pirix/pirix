#![feature(no_std, lang_items, asm, intrinsics, step_by, core_str_ext, core_slice_ext)]
#![no_std]

mod std {
    pub use core::fmt;
    pub use core::cmp;
    pub use core::ops;
    pub use core::iter;
    pub use core::option;
    pub use core::marker;
}

#[macro_use]
pub mod debug;
pub mod irq;
pub mod timer;
pub mod mem;
pub mod support;

#[path = "../arch/i386/mod.rs"]
pub mod arch;

#[no_mangle]
pub unsafe fn syscall() {

}

#[no_mangle]
pub unsafe fn scheduler_schedule() {

}

#[no_mangle]
pub fn main() {
    debug::init();
    debug::println("Booting Pirix 0.1");
    mem::init();
    arch::init();
    irq::init();
    timer::init();
    irq::start();
}

#[lang="panic_fmt"]
pub extern fn rust_begin_unwind(args: core::fmt::Arguments, file: &'static str, line: usize) -> ! {
    irq::stop();
    log!("Problem at {}:{}: {}", file, line, args);
    loop { };
}

#[lang="stack_exhausted"]
#[no_mangle]
pub fn __morestack() -> ! {
    loop {}
}

#[lang="eh_personality"]
#[no_mangle]
pub fn rust_eh_personality() -> ! {
  loop {}
}

#[no_mangle]
#[allow(non_snake_case)]
pub fn _Unwind_Resume()
{
	loop{}
}
