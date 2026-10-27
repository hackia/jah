#![cfg_attr(not(test), no_std, no_main)]

#[unsafe(no_mangle)]
pub extern "C" fn main() -> ! {
    loop {}
}

#[cfg_attr(not(test), panic_handler)]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}
