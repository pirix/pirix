use core::option::Option;

use arch;
use arch::cpu;

pub type IrqHandler = unsafe fn(&mut cpu::State);
static mut handlers: [Option<IrqHandler>; arch::IRQ_COUNT] = [Option::None; arch::IRQ_COUNT];

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

pub fn register(irq: usize, handler: IrqHandler) {
    unsafe {
        handlers[irq] = Option::Some(handler);
        arch::irq::allow(irq);
    }
}

pub fn unregister(irq: usize) {
    unsafe {
        handlers[irq] = Option::None;
        arch::irq::disallow(irq);
    }
}

#[no_mangle]
pub extern "C" fn irq_handle<'a>(state: &'a mut cpu::State) -> &'a mut cpu::State {
    let irq = state.irq as usize;

    unsafe {
        arch::irq::clear(irq);
        if handlers[irq].is_some() {
            let handler = handlers[irq].unwrap();
            handler(state);
        }
        else {
            panic!("unhandled interrupt {}", irq);
        }
    }

    return state;
}
