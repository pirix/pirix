pub use arch::debug::init;
pub use arch::debug::putchar;

use core::fmt;
use core::prelude::*;

struct DebugWriter;

impl DebugWriter {
    fn write_fmt(&mut self, fmt: &fmt::Arguments) {
        let _ = fmt::write(self, fmt);
    }
}

impl fmt::FormatWriter for DebugWriter {
    fn write(&mut self, bytes: &[u8]) -> fmt::Result {
        for &c in bytes.iter() {
            putchar(c as char);
        }
        Ok(())
    }
}

pub fn print(text: &str) {
    for c in text.chars() {
        putchar(c);
    }
}

pub fn println(text: &str) {
    print(text);
    putchar('\n');
}

pub fn print_args(fmt: &fmt::Arguments) {
    write!(DebugWriter, "{}", fmt);
}

pub fn println_args(fmt: &fmt::Arguments) {
    writeln!(DebugWriter, "{}", fmt);
}

macro_rules! print(
    ($($arg:tt)*) => (format_args!(::debug::print_args, $($arg)*))
)

macro_rules! println(
    ($($arg:tt)*) => (format_args!(::debug::println_args, $($arg)*))
)
