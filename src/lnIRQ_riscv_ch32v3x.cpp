/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "lnIRQ_riscv_priv_ch32v3x.h"
/**
 * 
 */

struct CH32V3_INTERRUPTx
{
    uint32_t ISR[4];                // 0x00 Interrupt Enable Status Register
    uint32_t dummy0[4];             // 0x10
    uint32_t IPR[4];                // 0x20 Interrupt Pending Status Register  
    uint32_t dummy1[4];             // 0x30
    uint32_t ITHRESHOLD;            // 0x40 Interrupt priority Threshold configuration register
    uint32_t dummy2;
    uint32_t CFGR      ;            // 0x48 Interrupt Configuration Register
    uint32_t GISR      ;            // 0x4C Global Status Register
    uint32_t VTFIDR    ;            // 0x50 VTF Interrupt ID configuration Register
    uint32_t dummy3[3]; 
    uint32_t VTFADDR[4]    ;        // 0x60 VTD Interrupt Address Register
    uint32_t dummy4[(0x100-0x70)/4]; 
    uint32_t IENR[4]    ;           // 0x100 Interrupt Enable Set Register
    uint32_t dummy5[(0x180-0x110)/4];
    uint32_t IRER[4]    ;           // 0x180 Interrupt Enable Reset Register
    uint32_t dummy6[(0x200-0x190)/4];
    uint32_t IPSR[4]    ;           // 0x200 Interrupt Pending Set Register
    uint32_t dummy7[(0x280-0x210)/4];
    uint32_t IPRR[4]    ;           // 0x280 Interrupt Pending Reset Register
    uint32_t dummy8[(0x300-0x290)/4];
    uint32_t IACTR[4]    ;          // 0x280 Interrupt Activation Register
    uint32_t dummy9[(0x400-0x310)/4];
    uint32_t IPRIOIR[64];          // 0x400 Priority(0..63)
};

typedef volatile CH32V3_INTERRUPTx CH32V3_INTERRUPT;

CH32V3_INTERRUPT *pfic = (CH32V3_INTERRUPT *)LN_PFIC_ADR;

uint32_t underInterrupt=0;

// Attribute [LEVEL1:LEVEL0][SHV] : 
//      LEVEL: 
//              0:0=LEVEL, 
//              0:1=RISING EDGE, 
//              1:0: FALLINGEDGE   
//      SHV:
//              0 non vectored
//              1 vectored
//
#define X(y) (uint32_t )y

void unsupported()
{
    deadEnd(11);
}
extern "C"
{
void SysTick_Handler();
void Ecall_M_Mode_Handler();
void Ecall_U_Mode_Handler();
void SW_Handler();
void USART0_IRQHandler();
void DMA0_Channel0_IRQHandler();
void DMA0_Channel1_IRQHandler();
void DMA0_Channel2_IRQHandler();
void DMA0_Channel3_IRQHandler();
void DMA0_Channel4_IRQHandler();
void DMA0_Channel5_IRQHandler();
void DMA0_Channel6_IRQHandler();
void DMA0_Channel7_IRQHandler();

void DMA1_Channel0_IRQHandler();
void DMA1_Channel1_IRQHandler();
void DMA1_Channel2_IRQHandler();
void DMA1_Channel3_IRQHandler();
void DMA1_Channel4_IRQHandler();
void DMA1_Channel5_IRQHandler();
void DMA1_Channel6_IRQHandler();
void DMA1_Channel7_IRQHandler();


}

static const uint32_t vecTable[]  __attribute__((aligned(32)))=
{
    X(unsupported),  //0 .word   RESET
    X(unsupported),  //1 .word   0
    X(unsupported), //2 .word   NMI_Handler                /* NMI */
    X(unsupported), //3 .word   HardFault_Handler          /* Hard Fault */
    X(unsupported), //4 .word   0
    X(unsupported), //5 .word   Ecall_M_Mode_Handler       /* Ecall M Mode */
    X(unsupported), //6 .word   0
    X(unsupported), //7 .word   0
    X(unsupported), //8 .word   Ecall_U_Mode_Handler       /* Ecall U Mode */
    X(unsupported), //9 .word   Break_Point_Handler        /* Break Point */
    X(unsupported), //10 .word   0
    X(unsupported), //11 .word   0
    X(SysTick_Handler), //12 .word   SysTick_Handler            /* SysTick */
    X(unsupported), //13 .word   0
    X(SW_Handler), //14 .word   SW_Handler                 /* SW */
    X(unsupported), //15 .word   0    
    X(unsupported), //.word   WWDG_IRQHandler            /* Window Watchdog */
    X(unsupported), //.word   PVD_IRQHandler             /* PVD through EXTI Line detect */
    X(unsupported), //.word   TAMPER_IRQHandler          /* TAMPER */
    X(unsupported), //.word   RTC_IRQHandler             /* RTC */
    X(unsupported), //.word   FLASH_IRQHandler           /* Flash */
    X(unsupported), //.word   RCC_IRQHandler             /* RCC */
    X(unsupported), //.word   EXTI0_IRQHandler           /* EXTI Line 0 */
    X(unsupported), //.word   EXTI1_IRQHandler           /* EXTI Line 1 */
    X(unsupported), //.word   EXTI2_IRQHandler           /* EXTI Line 2 */
    X(unsupported), //.word   EXTI3_IRQHandler           /* EXTI Line 3 */
    X(unsupported), //.word   EXTI4_IRQHandler           /* EXTI Line 4 */
    X(DMA0_Channel0_IRQHandler), //.word   DMA1_Channel1_IRQHandler   /* DMA1 Channel 1 */
    X(DMA0_Channel1_IRQHandler), //.word   DMA1_Channel2_IRQHandler   /* DMA1 Channel 2 */
    X(DMA0_Channel2_IRQHandler), //.word   DMA1_Channel3_IRQHandler   /* DMA1 Channel 3 */
    X(DMA0_Channel3_IRQHandler), //.word   DMA1_Channel4_IRQHandler   /* DMA1 Channel 4 */
    X(DMA0_Channel4_IRQHandler), //.word   DMA1_Channel5_IRQHandler   /* DMA1 Channel 5 */
    X(DMA0_Channel5_IRQHandler), //.word   DMA1_Channel6_IRQHandler   /* DMA1 Channel 6 */
    X(DMA0_Channel6_IRQHandler), //.word   DMA1_Channel7_IRQHandler   /* DMA1 Channel 7 */
    X(unsupported), //.word   ADC1_2_IRQHandler          /* ADC1_2 */
    X(unsupported), //.word   USB_HP_CAN1_TX_IRQHandler  /* USB HP and CAN1 TX */
    X(unsupported), //.word   USB_LP_CAN1_RX0_IRQHandler /* USB LP and CAN1RX0 */
    X(unsupported), //.word   CAN1_RX1_IRQHandler        /* CAN1 RX1 */
    X(unsupported), //.word   CAN1_SCE_IRQHandler        /* CAN1 SCE */
    X(unsupported), //.word   EXTI9_5_IRQHandler         /* EXTI Line 9..5 */
    X(unsupported), //.word   TIM1_BRK_IRQHandler        /* TIM1 Break */
    X(unsupported), //.word   TIM1_UP_IRQHandler         /* TIM1 Update */
    X(unsupported), //.word   TIM1_TRG_COM_IRQHandler    /* TIM1 Trigger and Commutation */
    X(unsupported), //.word   TIM1_CC_IRQHandler         /* TIM1 Capture Compare */
    X(unsupported), //.word   TIM2_IRQHandler            /* TIM2 */
    X(unsupported), //.word   TIM3_IRQHandler            /* TIM3 */
    X(unsupported), //.word   TIM4_IRQHandler            /* TIM4 */
    X(unsupported), //.word   I2C1_EV_IRQHandler         /* I2C1 Event */
    X(unsupported), //.word   I2C1_ER_IRQHandler         /* I2C1 Error */
    X(unsupported), //.word   I2C2_EV_IRQHandler         /* I2C2 Event */
    X(unsupported), //.word   I2C2_ER_IRQHandler         /* I2C2 Error */
    X(unsupported), //.word   SPI1_IRQHandler            /* SPI1 */
    X(unsupported), //.word   SPI2_IRQHandler            /* SPI2 */
    X(USART0_IRQHandler), //.word   USART1_IRQHandler          /* USART1 */
    X(unsupported), //.word   USART2_IRQHandler          /* USART2 */
    X(unsupported), //.word   USART3_IRQHandler          /* USART3 */
    X(unsupported), //.word   EXTI15_10_IRQHandler       /* EXTI Line 15..10 */
    X(unsupported), //.word   RTCAlarm_IRQHandler        /* RTC Alarm through EXTI Line */
    X(unsupported), //.word   USBWakeUp_IRQHandler       /* USB Wakeup from suspend */
    X(unsupported), //.word   TIM8_BRK_IRQHandler        /* TIM8 Break */
    X(unsupported), //.word   TIM8_UP_IRQHandler         /* TIM8 Update */
    X(unsupported), //.word   TIM8_TRG_COM_IRQHandler    /* TIM8 Trigger and Commutation */
    X(unsupported), //.word   TIM8_CC_IRQHandler         /* TIM8 Capture Compare */
    X(unsupported), //.word   RNG_IRQHandler             /* RNG */
    X(unsupported), //.word   FSMC_IRQHandler            /* FSMC */
    X(unsupported), //.word   SDIO_IRQHandler            /* SDIO */
    X(unsupported), //.word   TIM5_IRQHandler            /* TIM5 */
    X(unsupported), //.word   SPI3_IRQHandler            /* SPI3 */
    X(unsupported), //.word   UART4_IRQHandler           /* UART4 */
    X(unsupported), //.word   UART5_IRQHandler           /* UART5 */
    X(unsupported), //.word   TIM6_IRQHandler            /* TIM6 */
    X(unsupported), //.word   TIM7_IRQHandler            /* TIM7 */
    X(DMA1_Channel0_IRQHandler), //.word   DMA2_Channel1_IRQHandler   /* DMA2 Channel 1 */
    X(DMA1_Channel1_IRQHandler), //.word   DMA2_Channel2_IRQHandler   /* DMA2 Channel 2 */
    X(DMA1_Channel2_IRQHandler), //.word   DMA2_Channel3_IRQHandler   /* DMA2 Channel 3 */
    X(DMA1_Channel3_IRQHandler), //.word   DMA2_Channel4_IRQHandler   /* DMA2 Channel 4 */
    X(DMA1_Channel4_IRQHandler), //.word   DMA2_Channel5_IRQHandler   /* DMA2 Channel 5 */
    X(unsupported), //.word   ETH_IRQHandler             /* ETH */
    X(unsupported), //.word   ETH_WKUP_IRQHandler        /* ETH WakeUp */
    X(unsupported), //.word   CAN2_TX_IRQHandler         /* CAN2 TX */
    X(unsupported), //.word   CAN2_RX0_IRQHandler        /* CAN2 RX0 */
    X(unsupported), //.word   CAN2_RX1_IRQHandler        /* CAN2 RX1 */
    X(unsupported), //.word   CAN2_SCE_IRQHandler        /* CAN2 SCE */
    X(unsupported), //.word   OTG_FS_IRQHandler          /* OTGFS */
    X(unsupported), //.word   USBHSWakeup_IRQHandler     /* USBHS Wakeup */
    X(unsupported), //.word   USBHS_IRQHandler           /* USBHS */
    X(unsupported), //.word   DVP_IRQHandler             /* DVP */
    X(unsupported), //.word   UART6_IRQHandler           /* UART6 */
    X(unsupported), //.word   UART7_IRQHandler           /* UART7 */
    X(unsupported), //.word   UART8_IRQHandler           /* UART8 */
    X(unsupported), //.word   TIM9_BRK_IRQHandler        /* TIM9 Break */
    X(unsupported), //.word   TIM9_UP_IRQHandler         /* TIM9 Update */
    X(unsupported), //.word   TIM9_TRG_COM_IRQHandler    /* TIM9 Trigger and Commutation */
    X(unsupported), //.word   TIM9_CC_IRQHandler         /* TIM9 Capture Compare */
    X(unsupported), //.word   TIM10_BRK_IRQHandler       /* TIM10 Break */
    X(unsupported), //.word   TIM10_UP_IRQHandler        /* TIM10 Update */
    X(unsupported), //.word   TIM10_TRG_COM_IRQHandler   /* TIM10 Trigger and Commutation */
    X(unsupported), //.word   TIM10_CC_IRQHandler        /* TIM10 Capture Compare */
    X(DMA1_Channel5_IRQHandler), //.word   DMA2_Channel6_IRQHandler   /* DMA2 Channel 6 */
    X(DMA1_Channel6_IRQHandler), //.word   DMA2_Channel7_IRQHandler   /* DMA2 Channel 7 */
    X(unsupported), //.word   DMA2_Channel8_IRQHandler   /* DMA2 Channel 8 */
    X(unsupported), //.word   DMA2_Channel9_IRQHandler   /* DMA2 Channel 9 */
    X(unsupported), //.word   DMA2_Channel10_IRQHandler  /* DMA2 Channel 10 */
    X(unsupported), //.word   DMA2_Channel11_IRQHandler  /* DMA2 Channel 11 */
 
};

void lnIrqSysInit()
{
    // relocate vector table

 	asm volatile(   
                    /* Enable nested and hardware stack */
                    "li t0, 0x1f\n"
                    "csrw 0x804, t0\n" // INTSYSCR
                    /* Enable floating point and interrupt */
                    "li t0, 0x7800\n"
                    "csrs mstatus, t0\n"
                    /* Relocate vector table */
                    "la t0, vec\n"
                    "lw t0, 0(t0)\n"
                    "ori t0, t0, 3\n" //        <= [1] [0] 1: Absolute address 0: vectored    
	                "csrw mtvec, t0 \n" //
                    
                    "ret \n" //
                    ".align 4\n"
                    "vec : .dword %0"
                  :: "i"(vecTable)
                );
    return;
}
/**


*/
void _enableDisable(bool enableDisable, const LnIRQ &irq)
{
    int irq_num = _irqs[irq].irqNb;
    if(enableDisable)
    {
        pfic->IENR[irq_num >> 5] = 1<< (irq_num & 0x1f);
    }
    else
    {
        pfic->IRER[irq_num >> 5] = 1<< (irq_num & 0x1f);
    }
    // fence ?
}

/**
 * 
 * @param per
 */
void lnEnableInterrupt(const LnIRQ &irq)
{   
    _enableDisable(true,irq);   
}
void lnIrqSetPriority(const LnIRQ &irq, int prio )
{    
  //  lnSetInterruptLevelDirect(irq,1+prio,false);
}
/**
 * 
 * @param irq
 */
void lnSetInterruptLevelDirect(int irq, int prio, bool vectored)
{
    /*
    int irq_num = _irqDesc[irq];

_irqDesc
{
    LnIRQ       interrpt;
    int         irqNb;

    
    LN_ECLIC_irq *iclic=eclicIrqs+irq;
    bool attr=0;
    if(vectored) attr|=1;
    // Assume level interrupt (?)
    iclic->attr=attr;
    // control
    //  4 bits then 1 1 1 1
    // nlbits=4 => level = 4 bits, priority=0 bits
    iclic->control=(prio<<ECLIC_CTLBIT_SHIFT)+ECLIC_CTLBIT_LOW;    
    LN_FENCE();
    */
}
/**
 * 
 * @param per
 */
void lnDisableInterrupt(const LnIRQ &irq)
{
     _enableDisable(false,irq);
}

void lnWriteMthDirect(int mth)
{
   /* *eclicMth=mth;
    LN_FENCE();
    */
}
int  lnReadMthDirect()
{
    //return *eclicMth;
    return 0;
}

#define DMA_IRQ(d,c) extern "C" void DMA##d##_Channel##c##_IRQHandler(void) { dmaIrqHandler(d,c);}
/**
 * 
 * @param dma
 * @param channel
 */
void dmaIrqHandler(int dma, int channel);
DMA_IRQ(0,0)
DMA_IRQ(0,1)
DMA_IRQ(0,2)
DMA_IRQ(0,3)
DMA_IRQ(0,4)
DMA_IRQ(0,5)
DMA_IRQ(0,6)
DMA_IRQ(1,0)
DMA_IRQ(1,1)
DMA_IRQ(1,2)
DMA_IRQ(1,3)
DMA_IRQ(1,4)
DMA_IRQ(1,5)
DMA_IRQ(1,6)


#ifdef LN_ENABLE_I2C
 void i2cIrqHandler(int instance, bool error);
#else
  #define i2cIrqHandler(...) deadEnd(1)
#endif

#define I2C_IRQ(d) extern "C"{ void I2C##d##_EV_IRQHandler(void) LN_INTERRUPT_TYPE ; void I2C##d##_EV_IRQHandler(void) { i2cIrqHandler(d,false);}} \
                 extern "C"{ void I2C##d##_ERR_IRQHandler(void)LN_INTERRUPT_TYPE;  void I2C##d##_ERR_IRQHandler(void) { i2cIrqHandler(d,true);} }

I2C_IRQ(0)
I2C_IRQ(1)


extern "C" void deadEnd(int code)
{
    // No interrrupt
    ENTER_CRITICAL();
    while(1)
    {
        // blink red light...
        
    }
}        
// EOF

