use core::option::{Option, Some, None};

use arch;
use arch::cpu;

pub type IrqHandler = unsafe fn(&mut cpu::State);
static mut handlers: [Option<IrqHandler>, ..255] = [None, ..255];

pub fn init() {
    unsafe { arch::irq::init(); }
}

#[inline]
pub fn start() {
    unsafe { arch::irq::start(); }
}

#[inline]
pub fn stop() {
    unsafe { arch::irq::stop(); }
}

pub fn register(irq: uint, handler: IrqHandler) {
    unsafe {
        handlers[irq] = Some(handler);
        arch::irq::allow(irq);
    }
}

pub fn unregister(irq: uint) {
    unsafe {
        handlers[irq] = None;
        arch::irq::disallow(irq);
    }
}

#[no_mangle]
pub extern "C" fn irq_handle<'a>(state: &'a mut cpu::State) -> &'a mut cpu::State {
    let irq = state.irq as uint;

    unsafe {
        arch::irq::clear(irq);
        if handlers[irq].is_some() {
            let handler = handlers[irq].unwrap();
            handler(state);
        }
        else {
            println!("unhandled interrupt {}", irq);
        }
    }

    return state;
}
