use irq;
use arch::cpu;

static mut ticks: i32 = 0;

fn tick(_: &mut cpu::State) {
    unsafe {
        ticks += 1;

        if ticks % 20 == 0 {
            log!("tick {}", ticks);
        }
    }
}

pub fn init() {
    irq::register(0x20, tick);
}
