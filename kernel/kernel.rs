#![feature(lang_items, asm, intrinsics, const_fn)]
#![feature(box_syntax, alloc, collections)]
#![allow(unused_variables, dead_code)]
#![no_std]

extern crate rlibc;
extern crate spin;
extern crate alloc;
extern crate collections;

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
//pub mod irq;
//pub mod timer;
pub mod mem;
//pub mod process;
//pub mod thread;
//pub mod scheduler;
//pub mod syscall;

//#[path = "../arch/i386/mod.rs"]
//#[cfg(target_arch = "i386")]
//pub mod arch;

#[path = "../arch/x86_64/mod.rs"]
pub mod arch;

#[no_mangle]
pub fn main() {
    debug::init();
    debug::println("Booting Pirix 0.1");
    arch::init();
    mem::init();

    loop {}
    //irq::init();
    //timer::init();


    //let process = process::Process::new();
    //let x = process.borrow_mut();
    //x.exit(43);

    //irq::start();
}

#[lang="panic_fmt"]
pub extern fn rust_begin_unwind(args: core::fmt::Arguments, file: &'static str, line: usize) -> ! {
    //irq::stop();
    log!("Problem at {}:{}: {}", file, line, args);
    loop { };
}

#[lang="eh_personality"]
pub fn rust_eh_personality() { }
