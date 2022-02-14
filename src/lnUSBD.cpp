/*
 *  (C) 2022 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"

#include "lnUSBD.h"
#include "lnUSBD_priv.h"
#include "lnPinMapping.h"

#include "tusb_option.h"
#include "tusb_config.h"
#include "tusb_common.h"
#include "tusb_types.h"
/* Register
0x40005c00:     0x20    0x62    0x00    0x00    0x21    0x06    0x00    0x00
0x40005c08:     0x22    0x32    0x00    0x00    0x63    0xa2    0x00    0x00
0x40005c10:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c18:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c20:     0x00    0x9d    0x00    0x00    0x13    0x82    0x00    0x00
0x40005c28:     0x57    0xa3    0x00    0x00    0xb3    0x00    0x00    0x00
0x40005c30:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c38:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c40:     0x00    0x9d    0x00    0x00    0x13    0x82    0x00    0x00
0x40005c48:     0x66    0xa3    0x00    0x00    0xb3    0x00    0x00    0x00
0x40005c50:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c58:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c60:     0x00    0x9d    0x00    0x00    0x13    0x82    0x00    0x00
0x40005c68:     0x75    0x63    0x00    0x00    0xb3    0x00    0x00    0x00
0x40005c70:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c78:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c80:     0x00    0x9d    0x00    0x00    0x13    0x82    0x00    0x00
0x40005c88:     0x85    0xa3    0x00    0x00    0xb3    0x00    0x00    0x00
0x40005c90:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005c98:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005ca0:     0x00    0x9d    0x00    0x00    0x13    0x82    0x00    0x00
0x40005ca8:     0x94    0xa3    0x00    0x00    0xb3    0x00    0x00    0x00
0x40005cb0:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005cb8:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005cc0:     0x00    0x9d    0x00    0x00    0x13    0x82    0x00    0x00
0x40005cc8:     0xa3    0xa3    0x00    0x00    0xb3    0x00    0x00    0x00
0x40005cd0:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005cd8:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40005ce0:     0x00    0x9d    0x00    0x00    0x13    0x82    0x00    0x00
0x40005ce8:     0xb2    0xa3    0x00    0x00    0xb3    0x00    0x00    0x00
0x40005cf0:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00

*/
/* RAM
0x40006008:     0x40    0x00    0x00    0x00    0x00    0x84    0x00    0x00
0x40006010:     0xc0    0x00    0x00    0x00    0x08    0x00    0x00    0x00
0x40006018:     0xe5    0xf4    0x00    0x00    0xfe    0x9e    0x00    0x00
0x40006020:     0x08    0x01    0x00    0x00    0x40    0x00    0x00    0x00
0x40006028:     0xc8    0x00    0x00    0x00    0x00    0x84    0x00    0x00
0x40006030:     0x88    0x01    0x00    0x00    0x0d    0x00    0x00    0x00
0x40006038:     0x48    0x01    0x00    0x00    0x1f    0x40    0x00    0x00
0x40006040:     0x73    0x81    0x00    0x00    0x4b    0x36    0x00    0x00
0x40006048:     0xde    0xd9    0x00    0x00    0x37    0x3d    0x00    0x00
0x40006050:     0x7c    0x94    0x00    0x00    0x0d    0x43    0x00    0x00
0x40006058:     0xee    0xfd    0x00    0x00    0xac    0xa8    0x00    0x00
0x40006060:     0xc4    0x56    0x00    0x00    0x96    0x91    0x00    0x00
0x40006068:     0xaa    0xe2    0x00    0x00    0xd2    0x60    0x00    0x00
0x40006070:     0xd6    0x8d    0x00    0x00    0x2a    0xea    0x00    0x00
0x40006078:     0xb5    0xdf    0x00    0x00    0x5b    0xe5    0x00    0x00
0x40006080:     0x02    0x01    0x00    0x00    0x00    0x00    0x00    0x00
0x40006088:     0x83    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x40006090:     0x07    0x07    0x00    0x00    0xc5    0xde    0x00    0x00
0x40006098:     0xe0    0x4f    0x00    0x00    0x63    0xa0    0x00    0x00
0x400060a0:     0x93    0xcf    0x00    0x00    0xc1    0x46    0x00    0x00
0x400060a8:     0x87    0xa1    0x00    0x00    0xab    0x17    0x00    0x00
0x400060b0:     0xad    0xd3    0x00    0x00    0xa9    0x33    0x00    0x00
0x400060b8:     0x65    0x85    0x00    0x00    0xa1    0x3d    0x00    0x00
0x400060c0:     0x2d    0x36    0x00    0x00    0xea    0x26    0x00    0x00
0x400060c8:     0x05    0x75    0x00    0x00    0xe8    0x45    0x00    0x00


*/
#define MAX_ENDPOINT 8

LN_USBD_Registers      *aUSBD0=(LN_USBD_Registers *)(LN_USBD0_ADR);
volatile uint32_t      *aUSBD0_SRAM=(volatile uint32_t *)LN_USBD0_RAM_ADR;
static lnUsbDevice     *_usbInstance=NULL;
extern "C" void dcd_int_handler(int rhport);
// this is mapped at the start of of the SrAM
#define ENTRY_SIZE_T uint32_t 
struct BTableEntry
{
    volatile ENTRY_SIZE_T TxAdr;
    volatile ENTRY_SIZE_T TxSize;
    volatile ENTRY_SIZE_T RxAdr;
    volatile ENTRY_SIZE_T RxSize;
};
static BTableEntry *btables=(BTableEntry *)aUSBD0_SRAM;


static const tusb_desc_endpoint_t ep0OUT_desc =
{
  .bLength          = sizeof(tusb_desc_endpoint_t),
  .bDescriptorType  = TUSB_DESC_ENDPOINT,

  .bEndpointAddress = 0x00,
  .bmAttributes     = { .xfer = TUSB_XFER_CONTROL },
  .wMaxPacketSize   = CFG_TUD_ENDPOINT0_SIZE,
  .bInterval        = 0
};

static const tusb_desc_endpoint_t ep0IN_desc =
{
  .bLength          = sizeof(tusb_desc_endpoint_t),
  .bDescriptorType  = TUSB_DESC_ENDPOINT,

  .bEndpointAddress = 0x80,
  .bmAttributes     = { .xfer = TUSB_XFER_CONTROL },
  .wMaxPacketSize   = CFG_TUD_ENDPOINT0_SIZE,
  .bInterval        = 0
};


/**
  The Setup of shared ram is  (from bottom)
              8*8 => EndpointBufferdescriptor
       0x40   64 bytes Rx buffer  // 128 bytes per endpoint
       0x40   64 bytes Tx buffer
*/
int       ep_bufferTail;
int       ep_nbEp;
class EndPoints
{
public:
      enum EP_type
      {
          EP_NONE,
          EP_BULK,
          EP_CONTROL,
          EP_INTERRUPT,
      };
public:
      static void  reset()
      {
        ep_nbEp=0;
        ep_bufferTail=sizeof(BTableEntry)*MAX_ENDPOINT;
        for(int i=0;i<MAX_ENDPOINT;i++)
        {          
          aUSBD0->USBD_EPCS[i]=i+LN_USBD_EPxCS_RX_STA_DISABLED+LN_USBD_EPxCS_TX_STA_DISABLED;
        }
      }
      static bool  addEndpoint(int num, EP_type type, int size, bool rx)
      {
            //
            xAssert(!(size&1));
            
            uint32_t ep=0;
            switch(type)
            {
              case EP_BULK:     ep|=LN_USBD_EPxCS_EPCTL_BULK;break;
              case EP_CONTROL:  ep|=LN_USBD_EPxCS_EPCTL_CONTROL;break;
              case EP_INTERRUPT:ep|=LN_USBD_EPxCS_EPCTL_INTERRUPT;break;
              default: xAssert(0);break;
            }            
            //
            ep|=LN_USBD_EPxCS_EPADDR(num); // endpoint number
            
            BTableEntry *e=btables+ep_nbEp;
            e->RxAdr=ep_bufferTail;
            if(rx) 
            {
              int r=size;
              int inc;
              if(size<=62)
              {
                int n=((size-1)/2);
                r|=n<<10; // # of 2 bytes block
                inc=(n+1)*2;
              }else
              {
                int n=(size-31)/32;
                r|=(n<<10)+(1<<15);  // # of 32 bytes block
                inc=(n+1)*32;
              }
              e->RxSize=r;
              ep_bufferTail+=inc;
            }
            else // Tx
            {
              e->RxSize=0;
            }
            e->TxAdr=ep_bufferTail;
            if(!rx) // Tx 
            {
              e->TxSize=size;
              ep_bufferTail+=size;              
            }
            aUSBD0->USBD_EPCS[num&7]=ep+LN_USBD_EPxCS_RX_STA_VALID+LN_USBD_EPxCS_TX_STA_NAK;
            ep_nbEp++;
            return true;
      }
};


/**
*/
lnUsbDevice::lnUsbDevice(int port)
{
  xAssert(port==0);
  _handler=NULL;
  _usbInstance=this;  
}
/**
*/
lnUsbDevice::~lnUsbDevice()
{
  power(false);
  _usbInstance=NULL;
}
/**
*/
bool     lnUsbDevice:: init()
{
  
  // first disable interrupt
  irqEnabled(false);    
  // 48 Mhz input to usb
  lnPeripherals::enableUsb48Mhz();  
  //
  EndPoints::reset();
  // Switch pins... PA11 and PA12
  lnPinMode(PA12,lnINPUT_FLOATING); // D+
  lnPinMode(PA11,lnINPUT_FLOATING); // D-
  // Clear all interrupts
  aUSBD0->USBD_CTL&=0xff; // disable all interrupts  
  aUSBD0->USBD_DADDR=0; // address 0, disabled
  aUSBD0->USBD_BADDR=0; // Descriptors at the begining
  aUSBD0->USBD_CTL=LN_USBD_CTL_SETRST;  // reset
  lnDelayUs(100);
  aUSBD0->USBD_CTL=LN_USBD_CTL_CLOSE; // clear reset, go to closed dstate    
  aUSBD0->USBD_INTF&=~(LN_USBD_INTF_ERRIF + LN_USBD_INTF_RSTIF); // clear reset and error
  // Set buffer address
  uint32_t asLong=(uint32_t )0;
  xAssert(!(asLong&7)); // aligned normally  already  
  
  // And go, enable most IT
  // setup endpoint 0
  EndPoints::addEndpoint( 0,EndPoints::EP_CONTROL,64,true);
  EndPoints::addEndpoint( 0,EndPoints::EP_CONTROL,64,false);

//--
  //dcd_edpt_open (0, &ep0OUT_desc);
  //dcd_edpt_open (0, &ep0IN_desc);
  // go !
  setAddress(0);    
  //
#define GG(x) LN_USBD_CTL_##x
  aUSBD0->USBD_CTL= GG(RSTIE)+GG(ERRIE)+GG(PMOUIE)+GG(STIE)+GG(WKUPIE);//+GG(ESOFIE)++GG(SOFIE);
  lnDelayUs(20);
  aUSBD0->USBD_BADDR=asLong;   
  lnDelayUs(20);
  // clear state
  aUSBD0->USBD_INTF=0;
  lnDelayUs(20);
  if(  aUSBD0->USBD_INTF & LN_USBD_INTF_ERRIF)
  {
  //  xAssert(0);
    Logger("USBD error after setup\n");
    // Clear Error
      aUSBD0->USBD_INTF=~LN_USBD_CTL_ERRIE;
  }
  return true;
}
/**
*/
bool      lnUsbDevice::power(bool onoff)
{ 
  if(onoff)
  {
    lnPeripherals::enable(Peripherals::pUSB);   
    lnPeripherals::reset(Peripherals::pUSB);   
    
  }else
  {
    lnPeripherals::disable(Peripherals::pUSB);
  }
  return true;
}
/**
*/
bool      lnUsbDevice::irqEnabled(bool onoff)
{ 
  if(onoff)
  {
    lnEnableInterrupt(LN_IRQ_USB_HP_CAN_TX);
    lnEnableInterrupt(LN_IRQ_USB_LP_CAN_RX0);
    lnEnableInterrupt(LN_IRQ_USBWAKEUP);
    
  }else
  {
    lnDisableInterrupt(LN_IRQ_USB_HP_CAN_TX);
    lnDisableInterrupt(LN_IRQ_USB_LP_CAN_RX0);    
    lnDisableInterrupt(LN_IRQ_USBWAKEUP);
  }
  return true;
}
/**
*/
bool      lnUsbDevice::registerInterruptHandler(lnUsbIrqHandler *h)
{
    _handler=h; 
    return true;
}

bool lnUsbDevice::wakeUpHost()
{
  aUSBD0->USBD_CTL|= LN_USBD_CTL_RSREQ; // clear suspend
  return true;
}

/**
*/
bool      lnUsbDevice:: setAddress(int address)
{
  aUSBD0->USBD_DADDR=0x80+(address&0x7f); // address 0, enabled
  return true;
}
/**
*/
void USB_TX_IRQHandler()
{
  xAssert(_usbInstance);
  _usbInstance->txIrq();
}
/**
*/
void USB_RX_IRQHandler()
{
  xAssert(_usbInstance);
  _usbInstance->rxIrq();
}
void USB_WAKEUP_IRQHandler()
{
  xAssert(_usbInstance);
  _usbInstance->wakeUpIrq();
}

#define CLEAR_INTERRUPT(x)   {aUSBD0->USBD_INTF &=~(LN_USBD_INTF_##x); }
#define DISABLE_INTERRUPT(x)   {aUSBD0->USBD_CTL &=~(LN_USBD_INTF_##x); }

/**
*/
extern "C" void dcd_int_handler(int rh);
void      lnUsbDevice:: rxIrq()
{
  dcd_int_handler(0);
  
}
#if 0
  // What interrupt triggered ?
  static uint32_t flags_original=aUSBD0->USBD_INTF;
  static uint32_t flags_mask=aUSBD0->USBD_CTL;
  static uint32_t flags=(flags_original & flags_mask &0xff00);  
  if(flags & LN_USBD_INTF_STIF) // transfer complete
  {
    xAssert(0);
  }
  if(flags & LN_USBD_INTF_ERRIF) // error
  {
    //ifaUSBD0->USBD_INTF &= ~; // Clear error
    xAssert(0);
  }
  if(flags & LN_USBD_INTF_SPSIF) // suspend
  {
    CLEAR_INTERRUPT(SPSIF);
    return;
  }
  if(flags & LN_USBD_INTF_RSTIF) // Reset
  {
    CLEAR_INTERRUPT(RSTIF);
    DISABLE_INTERRUPT(RSTIF);
    
    return;
  }
  if(flags & LN_USBD_INTF_SOFIF) // start of frame
  {
      CLEAR_INTERRUPT(SOFIF);
      DISABLE_INTERRUPT(SOFIF);
      return;
  }
  if(flags)
    xAssert(0);

}
#endif    
void      lnUsbDevice:: txIrq()
{
  xAssert(0);
}
void      lnUsbDevice:: wakeUpIrq()
{
  xAssert(0);
}
// EOF
