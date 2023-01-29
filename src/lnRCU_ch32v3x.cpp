/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"
#include "lnPeripheral_priv.h"
#include "lnCpuID.h"
extern LN_RCU *arcu;
/**
    This is the USB HS clock for CH32V3x chip
*/
void lnPeripherals::enableUsbHS48Mhz_ch32v3x()
{
#if 0
  RCC_USBCLK48MConfig(RCC_USBCLK48MCLKSource_USBPHY);
  RCC_USBHSPLLCLKConfig(RCC_HSBHSPLLCLKSource_HSE);
  RCC_USBHSConfig(RCC_USBPLL_Div2);
  RCC_USBHSPLLCKREFCLKConfig(RCC_USBHSPLLCKREFCLK_4M);
  RCC_USBHSPHYPLLALIVEcmd(ENABLE);
#endif
  uint32_t cfg1=arcu->CFG1;
  cfg1&=0xffffff; 
  cfg1|=1<<27; // src = HSE
  cfg1|=(1<<24); // div = 2
  cfg1|=(1<<28); // Ref= 4Mhz
  cfg1|=(1<<31); // Src=USB PHY
  cfg1|=(1<<30); // USB PHY PLL ALive
  arcu->CFG1=cfg1;
}
// EOF
