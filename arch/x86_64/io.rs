pub fn outb(port: u16, value: u8) {
    unsafe {
        asm!("outb %al, %dx"
             : : "{dx}" (port), "{al}" (value)
             : : "volatile" );
    }
}

pub fn inb(port: u16) -> u8 {
    let ret: u8;
    unsafe {
        asm!("inb %dx, %al"
         : "={al}"(ret) : "{dx}"(port)
         : : "volatile" );
    }
    return ret;
}
