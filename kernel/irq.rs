use core::option::Option;

use arch;
use arch::cpu;

pub type IrqHandler = fn(&mut cpu::State);
static mut handlers: [Option<IrqHandler>; arch::IRQ_COUNT] = [Option::None; arch::IRQ_COUNT];

pub use arch::irq::{init, start, stop};

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
pub unsafe extern "C" fn irq_handle(state: &mut cpu::State) -> &mut cpu::State {
    let irq = state.irq as usize;

    arch::irq::clear(irq);

    if handlers[irq].is_some() {
        let handler = handlers[irq].unwrap();
        handler(state);
    }
    else {
        panic!("unhandled interrupt {}", irq);
    }

    return state;
}
