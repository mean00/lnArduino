#pragma once
#include "lnIRQ.h"
#include "lnPeripheral_priv.h"

/**
 */

struct LN_USBD_Registersx
{
    uint32_t USBD_EPCS[8];     // 0x00--0x1c
    uint32_t filler[(0x40-0x20)/4];
    uint32_t USBD_CTL;        // 0x40
    uint32_t USBD_INTF;       // 0x44
    uint32_t USBD_STAT;       // 0x48
    uint32_t USBD_DADDR;      // 0x4C
    uint32_t USBD_BADDR;      // 0x50
        
};

/**
 */
typedef volatile LN_USBD_Registersx LN_USBD_Registers;
/**
 */

//-------------- CTL ---------

#define LN_USBD_CTL_SETRST    (1<<0)
#define LN_USBD_CTL_CLOSE     (1<<1)
#define LN_USBD_CTL_LOWM      (1<<2)
#define LN_USBD_CTL_SETSPS    (1<<3)
#define LN_USBD_CTL_RSREQ     (1<<4)

#define LN_USBD_CTL_ESOFIE    (1<<8)
#define LN_USBD_CTL_SOFIE     (1<<9)
#define LN_USBD_CTL_RSTIE     (1<<10)
#define LN_USBD_CTL_SPEIE     (1<<11)
#define LN_USBD_CTL_WKUPIE    (1<<12)
#define LN_USBD_CTL_ERRIE     (1<<13)
#define LN_USBD_CTL_PMOUIE    (1<<14)
#define LN_USBD_CTL_STIE      (1<<15)

//-------------- INTF ---------

#define LN_USBD_INTF_EPNUM(x)  (x&0x0f)
#define LN_USBD_INTF_DIR       (1<<4)
#define LN_USBD_INTF_ESOFIF    (1<<8)
#define LN_USBD_INTF_SOFIF     (1<<9)
#define LN_USBD_INTF_RSTIF     (1<<10)
#define LN_USBD_INTF_SPSIF     (1<<11)
#define LN_USBD_INTF_WKUPIF    (1<<12)
#define LN_USBD_INTF_ERRIF     (1<<13)
#define LN_USBD_INTF_PMOUIF    (1<<14)
#define LN_USBD_INTF_STIF      (1<<15)


//-------------- STAT ---------

#define LN_USBD_STAT_FNCT(x)   (x&0x3ff)
#define LN_USBD_STAT_SOFLN(x)  ((x&3)<<11)
#define LN_USBD_STAT_LOCK      (1<<13)
#define LN_USBD_STAT_RX_DM     (1<<14)
#define LN_USBD_STAT_RX_DP     (1<<15)

// EOF


