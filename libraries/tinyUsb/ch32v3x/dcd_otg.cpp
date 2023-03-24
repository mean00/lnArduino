/* 
 * The MIT License (MIT)
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 
https://github.com/openwch/ch32v20x/blob/main/EVT/EXAM/USB/USBFS/DEVICE/CH372Device/User/ch32v20x_usbfs_device.c
 */
#include "lnArduino.h"
#include "lnIRQ.h"
#include "tusb_option.h"
#include "device/dcd.h"
#include "ch32_otg_reg.h"
#include "ch32_usbhs_reg.h"
#include "ch32_extra.h"
#undef USBHSD
#define CHECK_ALIGNED(x) { if(((uint32_t)x) &3) xAssert(0);}
LN_USB_OTG_DEVICE *USBOTGD =    (LN_USB_OTG_DEVICE *) USBOTG_BASE;
volatile uint8_t USBHS_Dev_Endp0_Tog = 0x01;

int nbTxComplete=0;
static uint8_t *getBufferAddress(int x, bool dir);
static void setEndpointMode(int endpoint, bool mod, bool enable_tx, bool enable_rx);
// Max number of bi-directional endpoints including EP0
#define EP_MAX 8

#if TUD_OPT_HIGH_SPEED
    #define MAXIMUM_PACKET_LEN  512
    #define SPEED_BITS          USBHS_HIGH_SPEED;
    #define REPORTED_SPEED      TUSB_SPEED_HIGH
#else
    #define MAXIMUM_PACKET_LEN  64
    #define SPEED_BITS          USBHS_FULL_SPEED
    #define REPORTED_SPEED      TUSB_SPEED_FULL
#endif   

// Max number of bi-directional endpoints including EP0

typedef struct {
    bool     active;
    uint8_t  *buffer;
    uint16_t total_len;
    uint16_t queued_len;
    uint16_t max_size;
    bool     short_packet;
} xfer_ctl_t;

typedef struct {
    xfer_ctl_t eps[EP_MAX*2];  // out then in
    uint8_t    buffer[EP_MAX][128]; // 2* 64 bytes out then in
} all_xfer_t;

static int xmin(int a, int b)
{
    if(a<b) return a;
    return b;
}

#define XFER_CTL_BASE(_ep, _dir) (&(xfer_status.eps[_ep*2+_dir]))
static  __attribute__((aligned(4))) all_xfer_t xfer_status;

/**
*/
void rxControl(int epnum, uint32_t mask, uint32_t set)
{
    uint32_t reg=USBOTGD->ep[epnum].rx_ctrl;
    reg&=~ (mask);
    reg|=set;
    USBOTGD->ep[epnum].rx_ctrl = reg;
}
void txControl(int epnum, uint32_t mask, uint32_t set)
{
    uint32_t reg=USBOTGD->ep[epnum].tx_ctrl;
    reg&=~ (mask);
    reg|=set;
    USBOTGD->ep[epnum].tx_ctrl = reg;
}
void txSet(int epnum, uint32_t val)
{
    USBOTGD->ep[epnum].tx_ctrl = val;
}
void rxSet(int epnum, uint32_t val)
{
    USBOTGD->ep[epnum].rx_ctrl = val;
}
void txLenSet(int epnum, uint32_t size)
{
    USBOTGD->ep[epnum].tx_length = size;
}

//------------


/**
*/
void dcd_init(uint8_t rhport) {
    (void)rhport;

    // enable clock
     lnPeripherals::enable(pUSBFS_OTG_CH32v3x);
     // 
    Logger("OTG FS driver\n");
    xDelay(1);
    USBOTGD->DEVICE_CTRL = 0;
    
    for(int i=1;i<8;i++) // ep0 is not configurable
    {
        setEndpointMode(i,false, true, true);        
    }

    for(int i=0;i<8;i++)
    {
        USBOTGD->dma[i]= (uint32_t )getBufferAddress(i,false);
    }

    USBOTGD->INT_FG = 0xFF;
    USBOTGD->INT_FG = 0xFF; // clear pending interrupts

    USBOTGD->CTRL = USBOTG_CTRL_RESET+USBOTG_CTRL_CLR_ALL;
    xDelay(1);
    USBOTGD->DEV_ADDRESS = 0;
    USBOTGD->CTRL = 0;
    USBOTGD->INT_EN = USBOTG_INT_EN_BUS_RESET_IE + USBOTG_INT_TRANSFER_IE; // + USBOTG_INT_NAK_IE;
    USBOTGD->CTRL = USBOTG_CTRL_PULLUP_ENABLE+ USBOTG_CTRL_DMA_ENABLE + USBOTG_CTRL_INT_BUSY;


//    txSet(0, USBHS_EP_T_RES_NAK);
//    rxSet(0, USBHS_EP_R_RES_ACK);

    USBOTGD->INT_FG = 0xff; // clear interrupt (again)

    USBOTGD->DEVICE_CTRL = USBOTG_DEVICE_CTRL_ENABLE;

}


uint8_t *getBufferAddress(int x, bool is_in)
{
    xAssert(x<8);
    uint8_t *s = xfer_status.buffer[x];
    return s+is_in*64;
}
/**
*/
void setEndpointMode(int endpoint, bool mod, bool enable_tx, bool enable_rx)
{
const uint8_t  offset[8] ={ 0,        0,  1 , 1, 0, 2,2, 3};
const uint8_t  up[8]     ={ 0,        1,  0,  1, 0, 0,1, 0};

    if(!endpoint) return;
    uint8_t value = USBOTGD->mod[offset[endpoint]];
    if(up[endpoint])
    {
        value&=0xF0;
        value|=  ( mod + enable_tx*4+enable_rx*8) <<4;
    }else
    {
        value&=0x0F;
        value|= ( mod + enable_tx*4+enable_rx*8) ;
    }
    USBOTGD->mod[offset[endpoint]]=value;
}
/**
*/
void setEndpointDmaAddress(int endpoint, uint32_t adr)
{
    USBOTGD->dma[endpoint] = adr;
}

/**
*/
void dcd_int_enable(uint8_t rhport) {
    (void)rhport;
    lnEnableInterrupt(LN_IRQ_OTG_FS);
}
/**
*/
void dcd_int_disable(uint8_t rhport) {
    (void)rhport;
    lnDisableInterrupt(LN_IRQ_OTG_FS);
}
/**
*/
void dcd_edpt_close_all(uint8_t rhport) {
    (void)rhport;
}
/**
*/
void dcd_set_address(uint8_t rhport, uint8_t dev_addr) {
    (void)dev_addr;

    // Response with zlp status
    dcd_edpt_xfer(rhport, 0x80, NULL, 0);
}
/**
*/
void dcd_remote_wakeup(uint8_t rhport)
{
  (void) rhport;
}

static void receive_packet(xfer_ctl_t *xfer, uint16_t xfer_size) {
    // xfer->queued_len = xfer->total_len - remaining;

    uint16_t remaining = xfer->total_len - xfer->queued_len;
    uint16_t to_recv_size;

    if (remaining <= xfer->max_size) {
        // Avoid buffer overflow.
        to_recv_size = (xfer_size > remaining) ? remaining : xfer_size;
    } else {
        // Room for full packet, choose recv_size based on what the microcontroller
        // claims.
        to_recv_size = (xfer_size > xfer->max_size) ? xfer->max_size : xfer_size;
    }

    if (to_recv_size) {
    }

    xfer->queued_len += xfer_size;

    // Per USB spec, a short OUT packet (including length 0) is always
    // indicative of the end of a transfer (at least for ctl, bulk, int).
    xfer->short_packet = (xfer_size < xfer->max_size);
}
/**
*/
void dcd_edpt0_status_complete(uint8_t rhport, tusb_control_request_t const *request) {
    (void)rhport;

  if (request->bmRequestType_bit.recipient == TUSB_REQ_RCPT_DEVICE &&
        request->bmRequestType_bit.type == TUSB_REQ_TYPE_STANDARD &&
        request->bRequest == TUSB_REQ_SET_ADDRESS) {
        USBOTGD->DEV_ADDRESS = (uint8_t)request->wValue;
    }

    txSet(0, USBHS_EP_T_RES_NAK);
    rxSet(0, USBHS_EP_R_RES_ACK);

}

/**
*/
bool dcd_edpt_open(uint8_t rhport, tusb_desc_endpoint_t const *desc_edpt) {
    (void)rhport;

    uint8_t const epnum = tu_edpt_number(desc_edpt->bEndpointAddress);
    uint8_t const dir = tu_edpt_dir(desc_edpt->bEndpointAddress);

    TU_ASSERT(epnum < EP_MAX);

    xfer_ctl_t *xfer = XFER_CTL_BASE(epnum, dir);
    xfer->max_size = tu_edpt_packet_size(desc_edpt);

    if (epnum != 0) {
        if (tu_edpt_dir(desc_edpt->bEndpointAddress) == TUSB_DIR_OUT) {
            rxSet(epnum, USBHS_EP_R_AUTOTOG | USBHS_EP_R_RES_ACK);
        } else {            
            txLenSet(epnum, 0);
            txSet(epnum, USBHS_EP_T_AUTOTOG | USBHS_EP_T_RES_NAK | USBHS_EP_T_TOG_0);
        }
    }

    return true;
}

/**
*/
int usbd_ep_close(const uint8_t ep) {
    (void)ep;

    return 0;
}
/**
*/
void dcd_edpt_stall(uint8_t rhport, uint8_t ep_addr) {
    (void)rhport;

   
}
/**
*/
void dcd_edpt_clear_stall(uint8_t rhport, uint8_t ep_addr) {
    (void)rhport;

   
}
/*
     ep_in => it's a write actually from the device point of view
*/
bool dcd_edpt_xfer_ep_in( xfer_ctl_t *xfer, uint8_t epnum, uint16_t total_bytes) 
{
    int short_packet_size = xmin(total_bytes, xfer->max_size);
    if(!epnum) // ep0
    {
        if(!total_bytes) // ep0 zlp
        {
            xfer->short_packet = true;
            txLenSet(0, 0);;                
        }else
        {
            xfer->queued_len = short_packet_size;
            txLenSet(0, short_packet_size);
            USBOTGD->dma[0]=(uint32_t )xfer->buffer;
        }
        txSet(0, USBHS_EP_T_RES_ACK | (USBHS_Dev_Endp0_Tog ? USBHS_EP_T_TOG_1 : USBHS_EP_T_TOG_0));
        USBHS_Dev_Endp0_Tog ^= 1;
        return true;
    }
    // Other EP
    if(!total_bytes) // epx zlp
    {
        xfer->short_packet = true;
        txLenSet(epnum, 0);        
        
    }else
    {
        xfer->queued_len = short_packet_size;               
        txLenSet(epnum,  short_packet_size);
        // copy to Tx dma
        memcpy( getBufferAddress(epnum,1), xfer->buffer, short_packet_size );        
    }
    txControl(epnum,USBHS_EP_T_RES_MASK ,USBHS_EP_T_RES_ACK); // go!
    return true;
}
/*
     ep_out => it's a read actually from the device point of view
*/
bool dcd_edpt_xfer_ep_out( xfer_ctl_t *xfer, uint8_t epnum, uint16_t total_bytes) 
{
    if (!epnum) // ep0
    {
        if(total_bytes)
        {
            uint32_t adr = (uint32_t )xfer->buffer;
            xAssert( (adr&7) ==0 );
            USBOTGD->dma[0]=adr;
        }
        rxControl(0,USBHS_EP_R_RES_MASK ,USBHS_EP_R_RES_ACK);
        return true;
    }
    // other op
    rxControl(epnum,USBHS_EP_R_RES_MASK ,USBHS_EP_R_RES_ACK);
    return true;
}
           
/**
*/
bool dcd_edpt_xfer(uint8_t rhport, uint8_t ep_addr, uint8_t *buffer, uint16_t total_bytes) {
    (void)rhport;
  
    
    uint8_t const epnum = tu_edpt_number(ep_addr);
    uint8_t const dir = tu_edpt_dir(ep_addr);

    xfer_ctl_t *xfer = XFER_CTL_BASE(epnum, dir);
    if(xfer->active)
    {
        xAssert(0);
    }
    xfer->active=true;
    xfer->buffer = buffer;    
    xfer->total_len = total_bytes;
    xfer->queued_len = 0;
    xfer->short_packet = false;
    
    if(dir) // ep_in
    {
        return dcd_edpt_xfer_ep_in(xfer, epnum,  total_bytes );
    }
    else 
    {
        return dcd_edpt_xfer_ep_out(xfer, epnum,  total_bytes );
    }
}

/**
*/
void dcd_int_handler(uint8_t rhport) {
    (void)rhport;

    
    uint8_t fg = USBOTGD->INT_FG;
    uint8_t st = USBOTGD->INT_ST;

    //
    if(fg &  USBOTG_INT_FG_BUS_RESET)
    {
        USBOTGD->INT_EN &= ~USBOTG_INT_EN_BUS_RESET_IE;
        XFER_CTL_BASE(0,0)->max_size = 64;
        XFER_CTL_BASE(0,1)->max_size = 64;
        USBOTGD->DEV_ADDRESS = 0;
        rxSet(0, USBHS_EP_R_RES_ACK | USBHS_EP_R_TOG_0);
        dcd_event_bus_reset(rhport, TUSB_SPEED_FULL, true);
        USBOTGD->INT_FG = USBOTG_INT_FG_BUS_RESET +(1<<3); // CLEAR SOF too
        return;
    }
    //
    if(fg & USBOTG_INT_FG_TRANSFER_COMPLETE)
    {
            int token_type = (st >> 4)&3; // 00 OUT Packet, 1 SOF, 2 IN packet , 3 SETUP packet
            int end_num = st & 0xf;
            uint8_t endp = end_num | (token_type == PID_IN ? TUSB_DIR_IN_MASK : 0);

            switch(token_type)
            {
                case PID_SOF:
                    USBOTGD->INT_FG = USBOTG_INT_FG_TRANSFER_COMPLETE;
                    return;      
                    break;
                case PID_OUT : 
                {
                    uint16_t rx_len = USBOTGD->RX_LEN;
                    if(end_num==0)
                    {
                        rxSet(0,  USBHS_EP_R_RES_ACK | USBHS_EP_R_TOG_0);                        
                        xfer_ctl_t *xfer = XFER_CTL_BASE(0, tu_edpt_dir(endp));
                        xfer->queued_len += rx_len;
                        dcd_event_xfer_complete(0, endp, xfer->queued_len, XFER_RESULT_SUCCESS, true);
                        if(rx_len==0) // zlp
                        {
                            USBOTGD->dma[0]=(uint32_t)getBufferAddress(0,false);
                        }
                    }else
                    {
                        rxControl(end_num, USBHS_EP_R_RES_MASK, USBHS_EP_R_RES_NAK );
                        xfer_ctl_t *xfer = XFER_CTL_BASE(end_num,false);
                        // copy from DMA area to final buffer
                        memcpy( xfer->buffer+ xfer->queued_len, getBufferAddress(end_num, false), rx_len);
                        xfer->queued_len += rx_len;
                        if ( rx_len < xfer->max_size || !xfer->total_len)
                        {
                             dcd_event_xfer_complete(0, endp, xfer->queued_len, XFER_RESULT_SUCCESS, true);
                        }else
                        {
                             rxControl(end_num,USBHS_EP_T_RES_MASK ,USBHS_EP_T_RES_ACK);
                        }
                    }
                    xAssert(0);
                    }
                    USBOTGD->INT_FG = USBOTG_INT_FG_TRANSFER_COMPLETE;
                    return;
                    break;
                case PID_IN : // IN
                {
                    xfer_ctl_t *xfer = XFER_CTL_BASE(end_num, tu_edpt_dir(endp));                    
                    xAssert(0);
                    USBOTGD->INT_FG = USBOTG_INT_FG_TRANSFER_COMPLETE;
                    return;
                }
                    break;
                case PID_SETUP : // SETUP
                {
                    USBHS_Dev_Endp0_Tog = 1;
                    dcd_event_setup_received(0, getBufferAddress(0,false), true);     
                    USBOTGD->INT_FG = USBOTG_INT_FG_TRANSFER_COMPLETE;
                    return;
                }
                    break;
                        
            }


    }
    USBOTGD->INT_FG = fg; // ack interrutp

}
int nbOtgIrq=0;

extern "C" void dcd_int_handler(uint8_t port);
extern "C" 
{
void OTG_FS_IRQHandler() LN_INTERRUPT_TYPE;
void OTG_FS_IRQHandler()
{
    nbOtgIrq++;
    dcd_int_handler(0);
}
}


