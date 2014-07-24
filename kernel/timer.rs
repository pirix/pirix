use irq;
use arch::cpu;
use arch::io::inb;

static mut ticks: i32 = 0;

unsafe fn tick(_: &mut cpu::State) {
    ticks += 1;

    if ticks % 20 == 0 {
        println!("tick {}", ticks);
    }
}

unsafe fn keyboard(_: &mut cpu::State) {
    let scancode = inb(0x60);
}

pub fn init() {
    irq::register(0x20, tick);
}
