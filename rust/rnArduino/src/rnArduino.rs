use embedded_hal as hal;
use core::convert::Infallible;
use hal::delay::DelayUs;
use crate::rnOsHelper::rnDelay;

struct rnArduino
{

}
///
/// 
/// 
/// 
impl DelayUs for rnArduino
{
    type Error = Infallible;

    fn delay_ms(&mut self, ms: u32) -> Result<(), Self::Error>
    {
        rnDelay(ms as u32);
        Ok(())
    }
    fn delay_us(&mut self, us: u32) -> Result<(), Self::Error>
    {
        rnDelay(us as u32);
        Ok(())
    }
}
