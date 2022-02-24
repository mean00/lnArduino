#pragma once
#define CFG_TUSB_MCU  OPT_MCU_STM32F1
#define MAX_EP_COUNT LN_USBD_MAX_ENDPOINT

#include "tusb_option.h"
#include "device/dcd.h"
#include "lnUSBD.h"
#include "lnUSBD_priv.h"
#include "lnUSBStack.h"
