pub use arch::debug::{init, putchar};

use core::fmt;

pub struct DebugWriter;

impl fmt::Write for DebugWriter {
    fn write_str(&mut self, text: &str) -> fmt::Result {
        print(text);
        Ok(())
    }
}

pub fn println(text: &str) {
    print(text);
    putchar('\n');
}

pub fn print(text: &str) {
    for c in text.chars() {
        putchar(c);
    }
}

macro_rules! log {
    ($($arg:tt)*) => ({
        use core::fmt::Write;
        let _ = writeln!(&mut ::debug::DebugWriter, "{}", format_args!($($arg)*));
    })
}
