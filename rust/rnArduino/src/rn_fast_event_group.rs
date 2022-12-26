#![allow(dead_code)]

use alloc::boxed::Box;
use crate::rnarduino as rn;

pub struct rnFastEventGroup
{
     ln : Box<rn::lnFastEventGroup> ,
}
impl Default for rnFastEventGroup 
{
       fn default() -> Self {
            Self::new()
         }
}
impl rnFastEventGroup
{
    pub fn new() -> Self
    {
        unsafe {
            let t: rnFastEventGroup = rnFastEventGroup{
                ln : Box::new(rn::lnFastEventGroup::new()),
            };
            t
        }
    }
   
        
    ///
    /// 
    /// 
    pub fn take_ownership(&mut self)
    {
        unsafe {
            self.ln.takeOwnership();
        }
    }
    ///
    /// 
    /// 
    pub fn set_events(&mut self, event : u32)
    {
        unsafe {
            self.ln.setEvents(event);
        }
    }
    ///
    /// 
    /// 
    pub fn wait_events(&mut self, maskint : u32, timeout : i32) -> u32
    {
        unsafe {
            self.ln.waitEvents(maskint,timeout)
        }
    } 
    ///
    /// 
    /// 
    pub fn read_events(&mut self, maskint : u32) -> u32
    {
        unsafe {
            self.ln.readEvents(maskint)
        }
    } 
}

//--


