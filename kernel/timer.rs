use irq;
use arch::cpu;

static mut ticks: i32 = 0;

unsafe fn tick(_: &mut cpu::State) {
    ticks += 1;

    if ticks % 20 == 0 {
        log!("tick {}", ticks);
    }
}

pub fn init() {
    irq::register(0x20, tick);
}
