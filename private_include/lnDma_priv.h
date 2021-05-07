/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once

#define DMA_CHAN_ENABLE         (1<<0)
#define DMA_CHAN_TFTFIE         (1<<1)      // transfer interrupt enable
#define DMA_CHAN_HTFIE          (1<<2)      // half interrupt enable
#define DMA_CHAN_ERRIE          (1<<3)      // error interrupt enable
#define DMA_CHAN_DIR_P2M        (0<<4)    // peripharl -> memory
#define DMA_CHAN_DIR_M2P        (1<<4)    // memory -> peripheral
#define DMA_CHAN_CMEN           (1<<5)       // circular
#define DMA_CHAN_PINCREASE      (1<<6)       // circular
#define DMA_CHAN_MINCREASE      (1<<7)       // circular
#define DMA_CHAN_PWIDTH_8       (0<<8)       // circular
#define DMA_CHAN_PWIDTH_16      (1<<8)       // circular
#define DMA_CHAN_PWIDTH_32      (2<<8)       // circular
#define DMA_CHAN_MWIDTH_8       (0<<10)       // circular
#define DMA_CHAN_MWIDTH_16      (1<<10)       // circular
#define DMA_CHAN_MWIDTH_32      (2<<10)       // circular

#define DMA_CHAN_PRIO_LOW       (0<<12)       // circular
#define DMA_CHAN_PRIO_MED       (1<<12)       // circular
#define DMA_CHAN_PRIO_HIGH      (2<<12)       // circular
#define DMA_CHAN_PRIO_UHIGH     (3<<12)       // circular

#define DMA_CHAN_MEM            (1<<14)       // circular

#define DMA_CHAN_KEEP_MASK      0xffff8000U