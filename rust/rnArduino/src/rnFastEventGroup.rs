#![allow(dead_code)]

use alloc::boxed::Box;
use crate::rnarduino as rn;

pub struct rnFastEventGroup
{
     ln : Box<rn::lnFastEventGroup> ,
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
    pub fn takeOwnership(&mut self) -> ()
    {
        unsafe {
            self.ln.takeOwnership();
        }
    }
    ///
    /// 
    /// 
    pub fn setEvents(&mut self, event : u32) -> ()
    {
        unsafe {
            self.ln.setEvents(event);
        }
    }
    ///
    /// 
    /// 
    pub fn waitEvents(&mut self, maskint : u32, timeout : i32) -> u32
    {
        unsafe {
            self.ln.waitEvents(maskint,timeout) as u32
        }
    } 
    ///
    /// 
    /// 
    pub fn readEvents(&mut self, maskint : u32) -> u32
    {
        unsafe {
            self.ln.readEvents(maskint) as u32
        }
    } 
}

//--


