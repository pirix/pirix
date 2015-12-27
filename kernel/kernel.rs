#![feature(lang_items, asm, intrinsics, const_fn)]
#![feature(box_syntax, alloc, collections)]
#![allow(unused_variables, dead_code)]
#![no_std]

extern crate rlibc;
extern crate spin;
extern crate alloc;
extern crate collections;

#[macro_use]
extern crate bitflags;

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

pub fn ping(state: &mut arch::cpu::State) {
    log!("ping 0x{:x}", arch::io::inb(0x60));
}

#[no_mangle]
pub fn main() {
    debug::init();
    debug::println("Booting Pirix 0.1");
    arch::init();
    irq::init();
    mem::init();
    irq::register(33, ping);
    irq::start();

    arch::paging::map(mem::Page { addr: 0x5000000 }, mem::Frame { addr: 0x2000 }, arch::paging::entry::WRITABLE);
    loop {}
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

#[no_mangle]
#[allow(non_snake_case)]
pub fn _Unwind_Resume() -> ! {
	loop { }
}
