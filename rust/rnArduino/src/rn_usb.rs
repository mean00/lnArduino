
#![allow(dead_code)]
use alloc::boxed::Box;
use crate::rnarduino as rn;

use crate::rn_usb_c::lnusb_c as usb_c;
use crate::rn_usb_c::{lnusb_create,lnusb_delete,lnusb_start,lnusb_stop,lnusb_setConfiguration, lnusb_setEventHandler};

use cty::{c_void,c_int};
//
//
pub enum usb_events
{
    CONNECT=0,
    DISCONNECT=1,
    SUSPEND=2,
    RESUME=3
}
//
trait usb_event_handler
{
    fn  handler(  &mut self,  _event : usb_events )
    {        
        panic!("oops");
    }
}

//--
pub struct rnUSB
{
     usb             : *mut usb_c,  
     handler         : Option< &'static mut dyn usb_event_handler>,       
}

impl usb_events
{
    pub fn from_u32( ix : u32) -> Self
    {
        return match ix {
           0 /*rn::lnUsbCDC_lnUsbCDCEvents::CDC_DATA_AVAILABLE*/ => usb_events::CONNECT,
           1 /*rn::lnUsbCDC_lnUsbCDCEvents::CDC_SESSION_START*/  => usb_events::DISCONNECT,
           2 /*rn::lnUsbCDC_lnUsbCDCEvents::CDC_SESSION_END*/    => usb_events::SUSPEND,
           3 /*rn::lnUsbCDC_lnUsbCDCEvents::CDC_SET_SPEED*/      => usb_events::RESUME,
            _ => panic!("EVENTCDC"),
        };
    }
}

/**
 * 
 * 
 */
impl Drop for rnUSB
{
    fn drop(&mut self) {
        unsafe {
            lnusb_delete(self.usb);
        }
    }
}
impl rnUSB
{
    // ctor
    pub fn new(instance : u32, handler : & 'static mut dyn usb_event_handler) -> Box<rnUSB>
    {
        unsafe {
        let r = Box::new(
            rnUSB {
                usb :  lnusb_create(instance),
                handler : Some(handler),
            }
        );
        let r_ptr = &*r as *const rnUSB;
        lnusb_setEventHandler(r.usb,r_ptr as *const c_void, Some(Self::bounceBack));
        r
        }        
       
    }

    //
    fn invoke_callback( &mut self, event : usb_events)
    {
        match &mut self.handler
            {
                Some(x) => x.handler( event),
                None    => panic!("no cdc handler"),
            }
    }
    /*
    *
    */
    extern "C" fn bounceBack(ptr : *mut cty::c_void,  event: c_int) -> ()
    {          
        //type cookie_monster <'a>=  Box<&'a rnCDC> ;
        type cookie_monster =   rnUSB;
        unsafe {  
        let  e : *mut cookie_monster = ptr as *mut  cookie_monster;
        //let  rf :& mut rnCDC =  &mut **e ;

        let a = &mut *e;
            //let r: () = a;
        a.invoke_callback(   usb_events::from_u32(event as u32));
           /* 
        //(*e).invoke_callback( instance as usize,  cdc_events::from_u32(event as u32),payload as u32);
        match rf
        {
            Some(mut x) => match x.handler
                        {
                            Some(mut y) => y.handler(instance as usize, cdc_events::from_u32(event as u32) , payload as u32),
                            None    => panic!("no cdc2"),
                        },
            None    => panic!("no CDC handler"),
        }*/
      }
    }
}
