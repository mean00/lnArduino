#![no_std]
use rnarduino::rn_os_helper;
use rnarduino::{lnLogger,lnLogger_init};

lnLogger_init!();

/**
 * 
 * 
 */
#[no_mangle]
extern "C" fn user_init() 
{
    lnLogger!("Hello there !\n");
    let number = 0x1234;
    lnLogger!("in decimal {}\n",number);
    lnLogger!("in hex {:x}\n",number);
    lnLogger!("--end--\n");
}

// EOF

