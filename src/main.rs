#![cfg_attr(not(test), no_std, no_main)]

#[unsafe(no_mangle)]
pub extern "C" fn main() -> ! {
    loop {}
}

#[cfg_attr(not(test), track_caller)]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}
