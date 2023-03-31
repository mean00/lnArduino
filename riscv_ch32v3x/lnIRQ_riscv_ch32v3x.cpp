/*
 *  (C) 2022/2023 MEAN00 fixounet@free.fr
 *  See license file
 *  PFIC is configured with 2 nested level, 1 bit for preemption
 *  That means interrupt priority between 0..7 ignoring preemption
 *
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "lnIRQ_riscv_priv_ch32v3x.h"
/**
 * 
 */
void PromoteIrqToFast(const LnIRQ &irq, int no);
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
    uint8_t  VTFIDR[4]    ;            // 0x50 VTF Interrupt ID configuration Register vector to assign to that fast vector
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
    uint32_t IACTR[4]    ;          // 0x300 Interrupt Activation Register
    uint32_t dummy9[(0x400-0x310)/4];
    uint32_t IPRIOIR[64];          // 0x400 Priority(0..63)
};

typedef volatile CH32V3_INTERRUPTx CH32V3_INTERRUPT;

CH32V3_INTERRUPT *pfic = (CH32V3_INTERRUPT *)LN_PFIC_ADR;
void _enableDisable_direct(bool enableDisable, const int &irq_num);

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

void  __attribute__ ((noinline)) unsupported()
{
    deadEnd(11);
}

void  __attribute__ ((noinline)) HardFault()
{
    deadEnd(12);
}


#define DECLARE_INTERRUPT(x) void x()         LN_INTERRUPT_TYPE;
extern "C"
{
DECLARE_INTERRUPT( SysTick_Handler)
DECLARE_INTERRUPT( Ecall_M_Mode_Handler)
DECLARE_INTERRUPT( Ecall_U_Mode_Handler)
DECLARE_INTERRUPT( SW_Handler)
DECLARE_INTERRUPT( USART0_IRQHandler)
DECLARE_INTERRUPT( DMA0_Channel0_IRQHandler)
DECLARE_INTERRUPT( DMA0_Channel1_IRQHandler)
DECLARE_INTERRUPT( DMA0_Channel2_IRQHandler)
DECLARE_INTERRUPT( DMA0_Channel3_IRQHandler)
DECLARE_INTERRUPT( DMA0_Channel4_IRQHandler)
DECLARE_INTERRUPT( DMA0_Channel5_IRQHandler)
DECLARE_INTERRUPT( DMA0_Channel6_IRQHandler)
DECLARE_INTERRUPT( DMA0_Channel7_IRQHandler)

DECLARE_INTERRUPT( DMA1_Channel0_IRQHandler)
DECLARE_INTERRUPT( DMA1_Channel1_IRQHandler)
DECLARE_INTERRUPT( DMA1_Channel2_IRQHandler)
DECLARE_INTERRUPT( DMA1_Channel3_IRQHandler)
DECLARE_INTERRUPT( DMA1_Channel4_IRQHandler)
DECLARE_INTERRUPT( DMA1_Channel5_IRQHandler)
DECLARE_INTERRUPT( DMA1_Channel6_IRQHandler)
DECLARE_INTERRUPT( DMA1_Channel7_IRQHandler)
DECLARE_INTERRUPT( USBHSWakeup_IRQHandler)
DECLARE_INTERRUPT( USBHS_IRQHandler)
DECLARE_INTERRUPT( OTG_FS_IRQHandler)

DECLARE_INTERRUPT(USART1_IRQHandler)
DECLARE_INTERRUPT(USART2_IRQHandler)

}
//extern void USB_TX_IRQHandler()     LN_INTERRUPT_TYPE;
//extern void USB_RX_IRQHandler()     LN_INTERRUPT_TYPE;

#ifdef LN_ENABLE_I2C
 void i2cIrqHandler(int instance, bool error);
#else
  #define i2cIrqHandler(...) deadEnd(1)
#endif

#define I2C_IRQ(d) extern "C"{ void I2C##d##_EV_IRQHandler(void) LN_INTERRUPT_TYPE ; void I2C##d##_EV_IRQHandler(void) { i2cIrqHandler(d,false);}} \
                 extern "C"{ void I2C##d##_ERR_IRQHandler(void)LN_INTERRUPT_TYPE;  void I2C##d##_ERR_IRQHandler(void) { i2cIrqHandler(d,true);} }

I2C_IRQ(0)
I2C_IRQ(1)

#define FAST_UNUSED   0xFF00
uint16_t fastInterrupt[4]={FAST_UNUSED, FAST_UNUSED, FAST_UNUSED, FAST_UNUSED};

static const uint32_t vecTable[]  __attribute__((aligned(32)))=
{
    X(unsupported),  //0 .word   RESET
    X(unsupported),  //1 .word   0
    X(unsupported), //2 .word   NMI_Handler                /* NMI */
    X(HardFault), //3 .word   HardFault_Handler          /* Hard Fault */
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
    X(unsupported), //20 .word   FLASH_IRQHandler           /* Flash */
    X(unsupported), //.word   RCC_IRQHandler             /* RCC */
    X(unsupported), //.word   EXTI0_IRQHandler           /* EXTI Line 0 */
    X(unsupported), //.word   EXTI1_IRQHandler           /* EXTI Line 1 */
    X(unsupported), //.word   EXTI2_IRQHandler           /* EXTI Line 2 */
    X(unsupported), //.word   EXTI3_IRQHandler           /* EXTI Line 3 */
    X(unsupported), //.word   EXTI4_IRQHandler           /* EXTI Line 4 */
    X(DMA0_Channel0_IRQHandler), //.word   DMA1_Channel1_IRQHandler   /* DMA1 Channel 1 */
    X(DMA0_Channel1_IRQHandler), //.word   DMA1_Channel2_IRQHandler   /* DMA1 Channel 2 */
    X(DMA0_Channel2_IRQHandler), //.word   DMA1_Channel3_IRQHandler   /* DMA1 Channel 3 */
    X(DMA0_Channel3_IRQHandler), // 30 .word   DMA1_Channel4_IRQHandler   /* DMA1 Channel 4 */
    X(DMA0_Channel4_IRQHandler), //.word   DMA1_Channel5_IRQHandler   /* DMA1 Channel 5 */
    X(DMA0_Channel5_IRQHandler), //.word   DMA1_Channel6_IRQHandler   /* DMA1 Channel 6 */
    X(DMA0_Channel6_IRQHandler), //.word   DMA1_Channel7_IRQHandler   /* DMA1 Channel 7 */
    X(unsupported), //.word   ADC1_2_IRQHandler          /* ADC1_2 */
    X(unsupported), //.word   USB_TX_IRQHandler USB_HP_CAN1_TX_IRQHandler  /* USB HP and CAN1 TX */
    X(unsupported), //.word   USB_RX_IRQHandler USB_LP_CAN1_RX0_IRQHandler /* USB LP and CAN1RX0 */
    X(unsupported), //.word   CAN1_RX1_IRQHandler        /* CAN1 RX1 */
    X(unsupported), //.word   CAN1_SCE_IRQHandler        /* CAN1 SCE */
    X(unsupported), //.word   EXTI9_5_IRQHandler         /* EXTI Line 9..5 */
    X(unsupported), //40 .word   TIM1_BRK_IRQHandler        /* TIM1 Break */
    X(unsupported), //.word   TIM1_UP_IRQHandler         /* TIM1 Update */
    X(unsupported), //.word   TIM1_TRG_COM_IRQHandler    /* TIM1 Trigger and Commutation */
    X(unsupported), //.word   TIM1_CC_IRQHandler         /* TIM1 Capture Compare */
    X(unsupported), //.word   TIM2_IRQHandler            /* TIM2 */
    X(unsupported), //.word   TIM3_IRQHandler            /* TIM3 */
    X(unsupported), //.word   TIM4_IRQHandler            /* TIM4 */
    X(I2C0_EV_IRQHandler), //.word   I2C1_EV_IRQHandler         /* I2C1 Event */
    X(I2C0_ERR_IRQHandler), //.word   I2C1_ER_IRQHandler         /* I2C1 Error */
    X(I2C1_EV_IRQHandler), //.word   I2C2_EV_IRQHandler         /* I2C2 Event */
    X(I2C1_ERR_IRQHandler), //.word   I2C2_ER_IRQHandler         /* I2C2 Error */
    X(unsupported), //.word   SPI1_IRQHandler            /* SPI1 */
    X(unsupported), //.word   SPI2_IRQHandler            /* SPI2 */
    X(USART0_IRQHandler), //.word   USART1_IRQHandler          /* USART1 */
    X(USART1_IRQHandler), //.word   USART2_IRQHandler          /* USART2 */
    X(USART2_IRQHandler), //.word   USART3_IRQHandler          /* USART3 */
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
    X(OTG_FS_IRQHandler), //.word   OTG_FS_IRQHandler          /* OTGFS */
    X(unsupported), //.word   USBHSWakeup_IRQHandler     /* USBHS Wakeup */
    X(USBHS_IRQHandler), //.word   USBHS_IRQHandler           /* USBHS */
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

// API used by the freeRTOS port
extern "C"
{
void NVIC_trigger_sw_irq()
{
  const uint32_t sw_irq=14;
  pfic->IPSR[sw_irq >> 5] = 1 << (sw_irq & 0x1F);
}
void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    _enableDisable_direct(true, IRQn);    
}
}
/**

*/
void lnIrqSysInit()
{
   // Disable fast
    for(int i=0;i<4;i++)
    {
        pfic->VTFIDR[i]=0;
        pfic->VTFADDR[i]=0;
        fastInterrupt[i]=FAST_UNUSED;
    }
    // Set all priorities to DEFAULT_PRIO (assuming 2 nested levels)
    #define DEFAULT_PRIO 5
    uint32_t prio32 = (DEFAULT_PRIO<<4) | (DEFAULT_PRIO<<(4+8)) | (DEFAULT_PRIO<<(4+16)) | (DEFAULT_PRIO<<(4+24));
    for(int i=0;i<64;i++)
    {
        pfic->IPRIOIR[i]=prio32;
    }

    // allow fast path for these 2 interrupts
    PromoteIrqToFast(LN_IRQ_SYSTICK, 1);
    PromoteIrqToFast(LN_IRQ_SW, 2);
    // Set these two to higher number = lower priority
    lnIrqSetPriority(LN_IRQ_SYSTICK,6);
    lnIrqSetPriority(LN_IRQ_SW,7);    

    // relocate vector table
    // Initialise WCH enhance interrutp controller, 
    uint32_t syscr=(0*CH32_SYSCR_HWSTKEN) | CH32_SYSCR_INESTEN | CH32_SYSCR_MPTCFG_2NESTED | CH32_SYSCR_HWSTKOVEN  ;
 	asm volatile(                       
                    "mv t0, %1\n"
                    "csrw 0x804, t0\n"   // INTSYSCR                    
                    "li t0, 0x7800\n"
                    "csrs mstatus, t0\n" // Enable floating point and interrupt 
                    "mv t0, %0 \n"
                    "ori t0, t0, 3\n"    //      Use vectored mode + relocate vector table
                    "csrw mtvec, t0 \n"  //                    
                  :: "r"(vecTable),"r"(syscr)
                );
   
    return;
}
/**

*/
bool xPortIsInsideInterrupt()
{
    uint32_t gisr = pfic -> GISR;    
    return (gisr>>8) & 1; // under interrupt
}

/**
*/
static int lookupIrq(int irq)
{
    if(_irqs[irq].interrpt==irq) return _irqs[irq].irqNb;
    // deep search
    int n=sizeof(_irqs)/sizeof(_irqDesc);
    for(int i=0;i<n;i++)
    {
          if(_irqs[i].interrpt==irq) return _irqs[i].irqNb;
    }
    xAssert(0);
    return 0;
}
/**
*/
void _enableDisable_direct(bool enableDisable, const int &irq_num)
{    
    if(enableDisable)
    {
        pfic->IENR[irq_num >> 5] = 1<< (irq_num & 0x1f); // 32 bits per register
    }
    else
    {
        pfic->IRER[irq_num >> 5] = 1<< (irq_num & 0x1f);
    }
    // fence ?
}
/**


*/
void _enableDisable(bool enableDisable, const LnIRQ &irq)
{
    int irq_num = lookupIrq(irq); //_irqs[irq].irqNb;
    _enableDisable_direct(enableDisable,irq_num);
}
/**
*/


/**

*/
void PromoteIrqToFast(const LnIRQ &irq, int no)
{
    if(no<1 || no > 4)
    {
        xAssert(0);
    }
    no--; // between 0 and 3 now
    int irq_num = lookupIrq(irq);
    uint32_t adr=vecTable[irq_num];
    fastInterrupt[no]=irq;
    pfic->VTFIDR[no]=irq_num;
    pfic->VTFADDR[no]=adr | 1; // fast path enabled by default, bit0
}


/**
 * 
 * @param per
 */
void lnEnableInterrupt(const LnIRQ &irq)
{   
    _enableDisable(true,irq);   
}


/**
    Set priority between 0 and 15
*/
void lnIrqSetPriority_direct(const int &irq_num, int prio )
{
    int s=(irq_num & 3)*8;
    int r=irq_num >> 2;
    uint32_t b = pfic->IPRIOIR[r];
    b&=~(0xff<<s);
    b|=(prio<<4) << s;
    pfic->IPRIOIR[r]=b;
}
/**

*/
void lnIrqSetPriority(const LnIRQ &irq, int prio )
{    
    int irq_num = lookupIrq(irq); //_irqs[irq].irqNb;
    lnIrqSetPriority_direct(irq_num,prio);
}
/**
 * 
 * @param per
 */
void lnDisableInterrupt(const LnIRQ &irq)
{
     _enableDisable(false,irq);
}


#define DMA_IRQ(d,c) \
            extern "C" void DMA##d##_Channel##c##_IRQHandler(void) LN_INTERRUPT_TYPE; \
            extern "C" void DMA##d##_Channel##c##_IRQHandler(void) { dmaIrqHandler(d,c);}

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



extern "C" void __attribute__ ((noinline))  deadEnd(int code)
{
    // No interrrupt
    ENTER_CRITICAL();
    while(1)
    {
        // blink red light...
        asm volatile("nop");
        
    }
}        


#define WEAK_INTERRUPT(x) void  __attribute__((weak))  x() {     xAssert(0); }

WEAK_INTERRUPT(USB_WAKEUP_IRQHandler)
WEAK_INTERRUPT(USB_TX_IRQHandler)
WEAK_INTERRUPT(USB_RX_IRQHandler)
WEAK_INTERRUPT(USBHS_IRQHandler)
WEAK_INTERRUPT(USART1_IRQHandler)
WEAK_INTERRUPT(USART2_IRQHandler)
WEAK_INTERRUPT(OTG_FS_IRQHandler)



void lnSoftSystemReset()
{
}
extern "C" int main();
extern "C" void __libc_init_array(void);
extern "C" void   __attribute__((noreturn))  start_c()
{
    __libc_init_array(); // call ctor before jumping in the code
    main();
    xAssert(0);  
}

// EOF


