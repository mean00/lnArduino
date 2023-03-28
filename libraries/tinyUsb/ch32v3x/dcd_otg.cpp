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
#include "ch32_extra.h"
#undef USBHSD
#define CHECK_ALIGNED(x) { if(((uint32_t)x) &3) xAssert(0);}
LN_USB_OTG_DEVICE *USBOTGD =    (LN_USB_OTG_DEVICE *) USBOTG_BASE;


#if 1
    #define LDEBUG Logger
#else
    #define LDEBUG(...)     {}
#endif

volatile uint8_t ep0_tx_tog = 0x01;
volatile uint8_t ep0_rx_tog = 0x01; 
int sept=0;
int tog_ko_out=0; /*-*/
int tog_ko_in=0;
int not_ready = 0;



int nbTxComplete=0;
static uint8_t *getBufferAddress(int x, bool dir);
// Max number of bi-directional endpoints including EP0
#define EP_MAX 8
#define MAXIMUM_PACKET_LEN  64
#define SPEED_BITS          USBOTG_FULL_SPEED
#define REPORTED_SPEED      TUSB_SPEED_FULL

// Max number of bi-directional endpoints including EP0

typedef struct {
    bool     active;
    uint8_t  *buffer;
    uint16_t total_len;
    uint16_t xfered_so_far;
    uint16_t max_size;
    uint16_t current_transfer;
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

#include "dcd_otg_misc.cpp"
/**
*/
void dcd_init(uint8_t rhport) {
    (void)rhport;

    // enable clock
    lnPeripherals::enable(pUSBFS_OTG_CH32v3x);
    // 
    LDEBUG("OTG FS driver\n");
    xDelay(1);
    USBOTGD->DEVICE_CTRL = 0;
    

    for(int i=0;i<EP_MAX;i++)
    {
        USBOTGD->dma[i]= (uint32_t )getBufferAddress(i,false);
    }
    XFER_CTL_BASE(0,0)->max_size = 64;
    XFER_CTL_BASE(0,1)->max_size = 64;

    USBOTGD->INT_FG = 0xFF;
    USBOTGD->INT_FG = 0xFF; // clear pending interrupts

    USBOTGD->CTRL = USBOTG_CTRL_RESET+USBOTG_CTRL_CLR_ALL;
    xDelay(1);
    USBOTGD->DEV_ADDRESS = 0;
    USBOTGD->CTRL = 0;
    USBOTGD->INT_EN = USBOTG_INT_EN_BUS_RESET_IE + USBOTG_INT_EN_TRANSFER_IE + USBOTG_INT_EN_SUSPEND_IE; // + USBOTG_INT_NAK_IE;
    USBOTGD->CTRL = USBOTG_CTRL_PULLUP_ENABLE+ USBOTG_CTRL_DMA_ENABLE + USBOTG_CTRL_INT_BUSY;


    txSet(0, USBOTG_EP_RES_NACK);
    rxSet(0, USBOTG_EP_RES_ACK);

    USBOTGD->INT_FG = 0xff; // clear interrupt (again)
    USBOTGD->DEVICE_CTRL = USBOTG_DEVICE_CTRL_ENABLE;

}

void dcd_set_address(uint8_t rhport, uint8_t dev_addr) 
{
    // Response with zlp status
    dcd_edpt_xfer(rhport, 0x80, NULL, 0);
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

    if (epnum != 0) 
    {
        if (tu_edpt_dir(desc_edpt->bEndpointAddress) == TUSB_DIR_OUT) {
            rxSet(epnum, USBOTG_EP_RES_AUTOTOG | USBOTG_EP_RES_ACK);
        } else {            
            txLenSet(epnum, 0);
            txSet(epnum, USBOTG_EP_RES_AUTOTOG | USBOTG_EP_RES_NACK | USBOTG_EP_RES_TOG0);
        }
    }
    return true;
}

/*
     ep_in => it's a write actually from the device point of view
*/
bool dcd_edpt_xfer_ep_in( xfer_ctl_t *xfer, uint8_t epnum) 
{
    LDEBUG("Prepare for IN with %d bytes on EP %d\n",xfer->total_len, epnum);
    int short_packet_size = xmin(xfer->total_len, xfer->max_size);
    if(!epnum) // ep0
    {
        if(!xfer->total_len) // ep0 zlp
        {            
            txLenSet(0, 0);
        }else
        {
            xfer->current_transfer = short_packet_size;
            txLenSet(0, short_packet_size);
            memcpy( getBufferAddress(0,1), xfer->buffer, short_packet_size );            
        }
        txSet(0, USBOTG_EP_RES_ACK | (ep0_tx_tog ? USBOTG_EP_RES_TOG1 : USBOTG_EP_RES_TOG0));
       /*/ if(short_packet_size == xfer->max_size)
        {
            ep0_tx_tog ^= 1;
        }
        */
        return true;
    }
    // Other EP
    if(!xfer->total_len) // epx zlp
    {
        xfer->current_transfer = 0;
        txLenSet(epnum, 0);                
    }else
    {
        xfer->current_transfer = short_packet_size;        
        txLenSet(epnum,  short_packet_size);
        // copy to Tx dma
        memcpy( getBufferAddress(epnum,1), xfer->buffer, short_packet_size );        
    }
    txControl(epnum,USBOTG_EP_RES_MASK ,USBOTG_EP_RES_ACK); // go!
    return true;
}
/*
     ep_out => it's a read actually from the device point of view
*/
bool dcd_edpt_xfer_ep_out( xfer_ctl_t *xfer, uint8_t epnum) 
{
    LDEBUG("Prepare for OUT with %d bytes on EP %d\n",xfer->total_len, epnum);
    if (!epnum) // ep0
    {
        if(xfer->total_len)
        {           
            if(xfer->total_len>xfer->max_size)
            {
                xAssert(0);
            }
        }        
        rxSet(0,USBOTG_EP_RES_ACK | ep0_rx_tog*USBOTG_EP_RES_TOG1);
        return true;
    }
    // other op
    rxControl(epnum,USBOTG_EP_RES_MASK ,USBOTG_EP_RES_ACK);
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
        //xAssert(0);
    }
    xfer->active=true;
    xfer->buffer = buffer;    
    xfer->total_len = total_bytes;
    xfer->xfered_so_far = 0;
    xfer->current_transfer = 0;
    
    if(dir) // ep_in
    {
        return dcd_edpt_xfer_ep_in(xfer, epnum );
    }
    else 
    {
        return dcd_edpt_xfer_ep_out(xfer, epnum );
    }
}
//
//
//
void dcd_int_reset()
{
    USBOTGD->INT_EN &= ~USBOTG_INT_EN_BUS_RESET_IE;
    XFER_CTL_BASE(0,0)->max_size = 64;
    XFER_CTL_BASE(0,1)->max_size = 64;
  //  USBOTGD->DEV_ADDRESS = 0;
    rxSet(0, USBOTG_EP_RES_NACK );
    txSet(0, USBOTG_EP_RES_NACK );
    txLenSet(0,0);

    // init ep to a safe value
    USBOTGD->dma[0] = (uint32_t )getBufferAddress(0,false);
    for(int ep = 1; ep< EP_MAX;ep++)
    {
        rxSet(ep, USBOTG_EP_RES_NACK | USBOTG_EP_RES_AUTOTOG );
        txSet(ep, USBOTG_EP_RES_NACK | USBOTG_EP_RES_AUTOTOG );
        USBOTGD->dma[ep] = (uint32_t )getBufferAddress(ep,false);
    }
    ep0_tx_tog = true;
    ep0_rx_tog = true;
    /*
    for(int i=1;i<EP_MAX;i++) // ep0 is not configurable
    {
        setEndpointMode(i,false, true, true);        
    }
    */
    USBOTGD->mod[0]=0xcc;
    USBOTGD->mod[1]=0xcc;
    USBOTGD->mod[2]=0xcc;
    USBOTGD->mod[3]=0xc;

    dcd_event_bus_reset(0, TUSB_SPEED_FULL, true);
}

extern "C" void dcd_edpt0_status_complete(uint8_t rhport, tusb_control_request_t const *request) {
    (void)rhport;

    if (request->bmRequestType_bit.recipient == TUSB_REQ_RCPT_DEVICE &&
        request->bmRequestType_bit.type == TUSB_REQ_TYPE_STANDARD &&
        request->bRequest == TUSB_REQ_SET_ADDRESS) 
    {
        USBOTGD->DEV_ADDRESS = (uint8_t)request->wValue;
    }

    //txSet(0, USBOTG_EP_RES_NACK);
    //rxSet(0, USBOTG_EP_RES_ACK);

}


/**
    It's ep_out so it's a read
*/
void dcd_int_out(int end_num)
{
    int rx_len = USBOTGD->RX_LEN;
    int endp = end_num;
    if(end_num==0)
    {
        rxSet(0,  USBOTG_EP_RES_NACK + ep0_rx_tog * USBOTG_EP_RES_TOG1);
        xfer_ctl_t *xfer = XFER_CTL_BASE(0, false);
        xfer->xfered_so_far += rx_len;
        dcd_event_xfer_complete(0, endp, xfer->xfered_so_far, XFER_RESULT_SUCCESS, true);
        if(rx_len==0) // zlp
        {            
            rxSet(0,  USBOTG_EP_RES_ACK + + ep0_rx_tog * USBOTG_EP_RES_TOG1);
            ep0_tx_tog = 1;
            ep0_rx_tog = 1;
        }else
        {
            if(xfer->buffer==NULL)
            {
                not_ready++;
            }
            ep0_rx_tog ^= 1;
        }
        return;
    }        
    xfer_ctl_t *xfer = XFER_CTL_BASE(end_num,false);
    // copy from DMA area to final buffer
    memcpy( xfer->buffer+ xfer->xfered_so_far, getBufferAddress(end_num, false), rx_len);
    xfer->xfered_so_far += rx_len;
    if ( rx_len < xfer->max_size ||  xfer->xfered_so_far==xfer->total_len)
    {
        rxControl(end_num, USBOTG_EP_RES_MASK, USBOTG_EP_RES_NACK );
        dcd_event_xfer_complete(0, endp, xfer->xfered_so_far, XFER_RESULT_SUCCESS, true);
    }else
    {
        rxControl(end_num,USBOTG_EP_RES_MASK ,USBOTG_EP_RES_ACK);
    }
    
}
//
// ep_in, it's a write
//
void dcd_int_in(int end_num)
{
    uint8_t endp = end_num |  TUSB_DIR_IN_MASK ;
    xfer_ctl_t *xfer = XFER_CTL_BASE(end_num, 1);                    
    if(end_num==0)
    {  // assume one transfer is enough (?)
        xfer->xfered_so_far+=xfer->current_transfer;        
        int left = xfer->total_len-xfer->xfered_so_far;
        ep0_tx_tog ^=1;
        xfer->current_transfer = 0;
        dcd_event_xfer_complete(0, endp  , xfer->xfered_so_far, XFER_RESULT_SUCCESS, true);

        txSet(0,  USBOTG_EP_RES_NACK + ep0_tx_tog * USBOTG_EP_RES_TOG1); //
        return;
    }   
    // finish or queue the next one ?
    xfer->xfered_so_far+=xfer->current_transfer;
    xfer->current_transfer = 0;
    int left = xfer->total_len-xfer->xfered_so_far;
    if(left>0)
    {
        left = xmin(left,xfer->max_size);
        memcpy( getBufferAddress(end_num, true), xfer->buffer+xfer->xfered_so_far, left);
        xfer->current_transfer = left;
        txLenSet(end_num, left);
        txControl(end_num, USBOTG_EP_RES_MASK, USBOTG_EP_RES_ACK);
    }else
    {
        txControl(end_num, USBOTG_EP_RES_MASK, USBOTG_EP_RES_NACK);
        dcd_event_xfer_complete(0, endp , xfer->xfered_so_far, XFER_RESULT_SUCCESS, true);
    }
        
    
}
/**
*/
void dcd_int_handler(uint8_t rhport) 
{
    (void)rhport;

    
    uint8_t fg = USBOTGD->INT_FG;
    uint8_t st = USBOTGD->INT_ST;

    //
    if(fg &  USBOTG_INT_FG_BUS_RESET)
    {
        dcd_int_reset();
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
                    break;
                case PID_OUT : // it's a read 
                    if(st & USBOTG_INT_ST_TOG_OK)
                    {
                        dcd_int_out(end_num);
                    }else
                    {
                        tog_ko_out++;
                    }
                    break;
                case PID_IN : // IN, it's a write
                    if(st & USBOTG_INT_ST_TOG_OK)
                    {
                        dcd_int_in(end_num);
                    }else
                    {
                        tog_ko_in++;
                    }
                    break;
                case PID_SETUP : // SETUP
                    ep0_tx_tog = 1;
                    ep0_rx_tog = 1;
                    dcd_event_setup_received(0, getBufferAddress(0,false), true);     
                    break;
                default:
                    xAssert(0);
                    break;
            }
            USBOTGD->INT_FG = USBOTG_INT_FG_TRANSFER_COMPLETE;
            return;      
                    
    }
    if(fg & USBOTG_INT_FG_SUSPEND)
    {
        USBOTGD->INT_FG = USBOTG_INT_FG_SUSPEND;
        return;      
    }
    xAssert(0);
}


