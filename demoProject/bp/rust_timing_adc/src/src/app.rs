use rn::rn_gpio::{pinMode, rnPin};
use rn::rn_timing_adc::rnTimingAdc;
use rnarduino as rn;
use rnarduino::{lnLogger, lnLogger_init};
const PIN_IN: rnPin = rnPin::PA3;

lnLogger_init!();
#[unsafe(no_mangle)]
pub extern "C" fn rnInit() {
    lnLogger!("Setuping up Timing ADC demo...\n");

    //pinMode(PS_PIN_VBAT          ,rn::rn_gpio::rnGpioMode::lnADC_MODE);
}

/**
 * \fn rnLoop
 *
 *
 *
 */

const SAMPLE_SIZE: usize = 32;
#[unsafe(no_mangle)]
pub extern "C" fn rnLoop() {
    lnLogger!("Running Timing ADC demo...\n");

    pinMode(PIN_IN, rn::rn_gpio::rnGpioMode::lnADC_MODE);
    let mut output: [u16; SAMPLE_SIZE] = [0; SAMPLE_SIZE];
    let pins: [rnPin; 1] = [PIN_IN];
    let mut adc = rnTimingAdc::new(0);
    adc.set_source(3, 3, 10000, &pins);
    loop {
        adc.multi_read(SAMPLE_SIZE as i32, &mut output);
        for i in 0..SAMPLE_SIZE {
            lnLogger!(" {} : {}\n", i, output[i]);
        }
        rn::rn_os_helper::delay_ms(1000);
    }
}
// EOF
