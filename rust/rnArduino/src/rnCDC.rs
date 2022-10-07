
#![allow(dead_code)]
use alloc::boxed::Box;

use crate::rnarduino as rn;
pub use rn::lnUsbCDC as  cdc;

type cint = cty::c_int;
type uint = cty::c_uint;


//--
pub trait rnCDCEventHandler
{
    fn  eventHandler( &self, interface: cint ,  event : u32, payload: u32);
}
//--
pub struct rnCDC
{
     ln             : Box<cdc> ,     
}

/**
 * 
 * 
 */
impl rnCDC 
{
    // ctor
    pub fn new(instance : u32, handler : dyn rnCDCEventHandler) -> rnCDC
    {
        unsafe {
            let t: rnCDC = rnCDC{
                ln :                Box::new(cdc::new(instance as  cint )),                  
            };
            t.ln._eventHandler= rnCDC::bounceBack;
            t.ln._eventCookie = handler;
            t
        }
    }
    // returns byte received
    pub fn read( &mut self,data : &mut [u8]) -> i32
    {
        unsafe{
        return self.ln.read(data.as_ptr() as *mut u8 ,data.len() as  cty::c_int) as i32;
        }
    }
    // returns byte received
    pub fn write( &mut self , data : &[u8]) -> i32
    {
        unsafe{
            return self.ln.write(data.as_ptr()  ,data.len() as  cty::c_int) as i32;
            }
    }
    // flush
    pub fn flush( &mut self )
    {
        unsafe{
                self.ln.flush();
            }
    }

    fn bounceBack(ptr : *mut cty::c_void, instance : cint, event: rn::lnUsbCDC_lnUsbCDCEventsHandler , payload : cty::c_uint) -> ()
    {
        // ptr is actually a rnCDCEventHandler in disguise
        let e = ptr as *rnCDCEventHandler;
        e.eventHandler(instance,event,payload);
    }
}