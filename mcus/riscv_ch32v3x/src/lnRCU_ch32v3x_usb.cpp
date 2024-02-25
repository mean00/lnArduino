/**
 * @file lnRCU_ch32v3x_usb.cpp
 * @author 2021/2024 MEAN00
 * @brief USB HS clock management
 *
 * @copyright Copyright (c) 2021/2024
 */
#include "lnArduino.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"
extern LN_RCU *arcu;

#define CH32V3_USBHS_PLL_SRC_HSI (0 << 27)
#define CH32V3_USBHS_PLL_SRC_HSE (1 << 27) // the datasheet is wrong
#define CH32V3_USBHS_PREFIX(x) ((x) << 24)
#define CH32V3_USBHS_CKREF_SEL(x) ((x) << 28)
#define CH32V3_USBHS_CKREF_SRC_USB_PHY (1 << 31)
#define CH32V3_USBHS_CKREF_SRC_PLL (0 << 31)
#define CH32V3_USBHS_USB_PHY_ENABLED (1 << 30)
#define CH32V3_USBHS_USB_PHY_DISABLED (0 << 30)

/**
    \brief This is the USB HS clock for CH32V3x chip
    NB: The CFG1 register is called RCC_CFGR2 in the datasheet
*/
void lnPeripherals::enableUsbHS48Mhz_ch32v3x()
{
    uint32_t cfgr2 = arcu->CFG1;
    cfgr2 &= 0xffffff;
    cfgr2 |= CH32V3_USBHS_PLL_SRC_HSE;       // src = HSE
    cfgr2 |= CH32V3_USBHS_PREFIX(1);         // 0= no div, 1=div by 2
    cfgr2 |= CH32V3_USBHS_CKREF_SEL(1);      // Ref= 4Mhz
    cfgr2 |= CH32V3_USBHS_CKREF_SRC_USB_PHY; // Src=USB PHY
    cfgr2 |= CH32V3_USBHS_USB_PHY_ENABLED;   // USB PHY PLL ALive
    arcu->CFG1 = cfgr2;
}
// EOF
