
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

// Out then in
uint8_t *getBufferAddress(int x, bool is_in)
{
    xAssert(x<8);
    uint8_t *s = xfer_status.buffer[x];
    if(x)                   // shared TX/RX buffer for EP0
        return s+is_in*64;
    return s;
}
/**
0 not used
   1 4
   2 3
   5 6
   7
*/
//                          0         1   2   3  4  5 6  7    
const uint8_t  offset[8] ={ 0,        0,  1 , 1, 0, 2,2, 3};
const uint8_t  up[8]     ={ 0,        1,  0,  1, 0, 0,1, 0};

void setEndpointMode(int endpoint, bool mod, bool enable_tx, bool enable_rx)
{

    if(!endpoint) return;
    volatile uint8_t *adr = &(USBOTGD->mod[offset[endpoint]]);
    uint8_t value = *adr;
    if(up[endpoint])
    {
        value&=0x0F;
        value|=  ( mod + enable_tx*4+enable_rx*8)<<4;
    }else
    {
        value&=0xF0;
        value|= ( mod + enable_tx*4+enable_rx*8) ;
    }
    *adr=value;
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
/**
*/
void dcd_remote_wakeup(uint8_t rhport)
{
  (void) rhport;
  xAssert(0);
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

   xAssert(0);
}
/**
*/
void dcd_edpt_clear_stall(uint8_t rhport, uint8_t ep_addr) {
    (void)rhport;
    xAssert(0);
   
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
// EOF