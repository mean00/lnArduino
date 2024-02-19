#ifndef USB_CH32_USBFS_REG_H
#define USB_CH32_USBFS_REG_H

// MEANX #if (CFG_TUSB_MCU == OPT_MCU_CH32V20X)
// MEANX #include <ch32v20x.h>
// MEANX #endif
#include "ch32_extra.h"

// CTRL
#define USBFS_CTRL_DMA_EN    (1 << 0)
#define USBFS_CTRL_CLR_ALL   (1 << 1)
#define USBFS_CTRL_RESET_SIE (1 << 2)
#define USBFS_CTRL_INT_BUSY  (1 << 3)
#define USBFS_CTRL_SYS_CTRL  (1 << 4)
#define USBFS_CTRL_DEV_PUEN  (1 << 5)
#define USBFS_CTRL_LOW_SPEED (1 << 6)
#define USBFS_CTRL_HOST_MODE (1 << 7)

// INT_EN
#define USBFS_INT_EN_BUS_RST  (1 << 0)
#define USBFS_INT_EN_DETECT   (1 << 0)
#define USBFS_INT_EN_TRANSFER (1 << 1)
#define USBFS_INT_EN_SUSPEND  (1 << 2)
#define USBFS_INT_EN_HST_SOF  (1 << 3)
#define USBFS_INT_EN_FIFO_OV  (1 << 4)
#define USBFS_INT_EN_DEV_NAK  (1 << 6)
#define USBFS_INT_EN_DEV_SOF  (1 << 7)

// INT_FG
#define USBFS_INT_FG_BUS_RST  (1 << 0)
#define USBFS_INT_FG_DETECT   (1 << 0)
#define USBFS_INT_FG_TRANSFER (1 << 1)
#define USBFS_INT_FG_SUSPEND  (1 << 2)
#define USBFS_INT_FG_HST_SOF  (1 << 3)
#define USBFS_INT_FG_FIFO_OV  (1 << 4)
#define USBFS_INT_FG_SIE_FREE (1 << 5)
#define USBFS_INT_FG_TOG_OK   (1 << 6)
#define USBFS_INT_FG_IS_NAK   (1 << 7)

// INT_ST
#define USBFS_INT_ST_MASK_UIS_ENDP(x)  (((x) >> 0) & 0x0F)
#define USBFS_INT_ST_MASK_UIS_TOKEN(x) (((x) >> 4) & 0x03)

// UDEV_CTRL
#define USBFS_UDEV_CTRL_PORT_EN   (1 << 0)
#define USBFS_UDEV_CTRL_GP_BIT    (1 << 1)
#define USBFS_UDEV_CTRL_LOW_SPEED (1 << 2)
#define USBFS_UDEV_CTRL_DM_PIN    (1 << 4)
#define USBFS_UDEV_CTRL_DP_PIN    (1 << 5)
#define USBFS_UDEV_CTRL_PD_DIS    (1 << 7)

// TX_CTRL
#define USBFS_EP_T_RES_MASK (3 << 0)
#define USBFS_EP_T_TOG      (1 << 2)
#define USBFS_EP_T_AUTO_TOG (1 << 3)

#define USBFS_EP_T_RES_ACK   (0 << 0)
#define USBFS_EP_T_RES_NYET  (1 << 0)
#define USBFS_EP_T_RES_NAK   (2 << 0)
#define USBFS_EP_T_RES_STALL (3 << 0)

// RX_CTRL
#define USBFS_EP_R_RES_MASK (3 << 0)
#define USBFS_EP_R_TOG      (1 << 2)
#define USBFS_EP_R_AUTO_TOG (1 << 3)

#define USBFS_EP_R_RES_ACK   (0 << 0)
#define USBFS_EP_R_RES_NYET  (1 << 0)
#define USBFS_EP_R_RES_NAK   (2 << 0)
#define USBFS_EP_R_RES_STALL (3 << 0)

// token PID
#define PID_OUT   0
#define PID_SOF   1
#define PID_IN    2
#define PID_SETUP 3

#endif // USB_CH32_USBFS_REG_H
