#![crate_name = "kernel"]
#![license = "GPL3"]
#![crate_type = "staticlib"]
#![no_std]
#![feature(asm, globs, phase, lang_items, intrinsics, macro_rules)]

#[phase(plugin, link)]
extern crate core;
use core::fmt;

mod std {
    pub use core::fmt;
    pub use core::option;
}

#[path = "../arch/i386/mod.rs"]
pub mod arch;

pub mod debug;
pub mod irq;
pub mod timer;
pub mod mem;
pub mod support;

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
    irq::init();
    timer::init();
    debug::println("Interrupts started");
}


#[lang = "stack_exhausted"] extern fn stack_exhausted() {}
#[lang = "eh_personality"] extern fn eh_personality() {}
#[lang="begin_unwind"]
unsafe extern "C" fn begin_unwind(fmt: &fmt::Arguments, file: &str, line: uint) -> ! {
    debug::println("Problem!");
    loop { };
}
