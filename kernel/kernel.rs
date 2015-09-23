#![feature(no_std, lang_items, asm, intrinsics, step_by, core_str_ext, core_slice_ext, box_syntax)]
#![allow(unused_variables, dead_code)]
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
pub mod process;
pub mod thread;
pub mod scheduler;
pub mod syscall;

#[path = "../arch/i386/mod.rs"]
pub mod arch;

#[no_mangle]
pub fn main() {
    debug::init();
    debug::println("Booting Pirix 0.1");
    mem::init();
    arch::init();
    irq::init();
    timer::init();


    let process = process::Process::new();
    let x = process.borrow_mut();
    let y = process.borrow_mut();
    x.exit(42);
    y.exit(43);

    irq::start();
}

#[lang="panic_fmt"]
pub extern fn rust_begin_unwind(args: core::fmt::Arguments, file: &'static str, line: usize) -> ! {
    irq::stop();
    log!("Problem at {}:{}: {}", file, line, args);
    loop { };
}
