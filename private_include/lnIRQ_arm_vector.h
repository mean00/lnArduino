/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once

#define VEC(x) ((uint32_t)x)

static const uint32_t LnVectorTable[]  __attribute__((aligned(256)))
={
VEC(&msp[LN_MSP_SIZE_UINT32-1]), /* 0 LN_IRQ_MSI=-13, */
VEC(__exc_reset),       /*1 LN_IRQ_RESET                                                                           */
VEC(NMI_IrqHandler),       /*2 LN_IRQ_NMI                                                                           */
VEC(HardFault_IrqHandler),       /*3 LN_IRQ_HARDFAULT                                                                           */
VEC(MemManage_IrqHandler),       /*4 LN_IRQ_MEMMANAGE                                                                           */
VEC(BusFault_IrqHandler),       /*5 LN_IRQ_BUSFAULT                                                                           */
VEC(UsageFault_IrqHandler),       /*6 LN_IRQ_USAGEFAULT                                                                           */
VEC(unsupported),       /*7 LN_IRQ_RESERVED                                                                           */
VEC(unsupported),       /*8 LN_IRQ_RESERVED2                                                                           */
VEC(unsupported),       /*9 LN_IRQ_RESERVED3                                                                           */
VEC(unsupported),       /*10 LN_IRQ_RESERVED4                                                                           */
VEC(vPortSVCHandler),   /*11 LN_IRQ_SVCALL                                                                           */
VEC(DebugMon_IrqHandler),       /*12 LN_IRQ_DEBUGMON                                                                           */
VEC(unsupported),       /*13 LN_IRQ_RESERVED5                                                                           */
VEC(xPortPendSVHandler),/*14 LN_IRQ_PENDSV                                                                           */
VEC(xPortSysTickHandler),/*15 LN_IRQ_SYSTICK                                                                           */

/* REAL IRQS */
VEC(WDG_IrqHandler), /*LN_IRQ_WDGT               !< window watchDog timer interrupt                          */
VEC(unsupported), /*LN_IRQ_LVD                 !< LVD through EXTI line detect interrupt                   */
VEC(Tamper_IrqHandler), /*LN_IRQ_TAMPER              !< tamper through EXTI line detect                          */
VEC(RTC_IrqHandler), /*LN_IRQ_RTC                 !< RTC alarm interrupt                                      */
VEC(FMC_IrqHandler), /*LN_IRQ_FMC                 !< FMC interrupt                                            */
VEC(RCU_IrqHandler), /*LN_IRQ_RCU_CTC             !< RCU and CTC interrupt                                    */
VEC(EXTI0_IRQHandler), /*LN_IRQ_EXTI0               !< EXTI line 0 interrupts                                   */
VEC(EXTI1_IRQHandler), /*LN_IRQ_EXTI1               !< 20 EXTI line 1 interrupts                                   */
VEC(EXTI2_IRQHandler), /*LN_IRQ_EXTI2               !< EXTI line 2 interrupts                                   */
VEC(EXTI3_IRQHandler), /*LN_IRQ_EXTI3               !< EXTI line 3 interrupts                                   */
VEC(EXTI4_IRQHandler), /*LN_IRQ_EXTI4               !< EXTI line 4 interrupts                                   */
VEC(DMA0_Channel0_IRQHandler), /*LN_IRQ_DMA0_Channel0       !< DMA0 channel0 interrupt                                  */
VEC(DMA0_Channel1_IRQHandler), /*LN_IRQ_DMA0_Channel1       !< DMA0 channel1 interrupt                                  */
VEC(DMA0_Channel2_IRQHandler), /*LN_IRQ_DMA0_Channel2       !< DMA0 channel2 interrupt                                  */
VEC(DMA0_Channel3_IRQHandler), /*LN_IRQ_DMA0_Channel3       !< DMA0 channel3 interrupt                                  */
VEC(DMA0_Channel4_IRQHandler), /*LN_IRQ_DMA0_Channel4       !< DMA0 channel4 interrupt                                  */
VEC(DMA0_Channel5_IRQHandler), /*LN_IRQ_DMA0_Channel5       !< DMA0 channel5 interrupt                                  */
VEC(DMA0_Channel6_IRQHandler), /*LN_IRQ_DMA0_Channel6       !< 30 DMA0 channel6 interrupt                                  */
VEC(ADC01_IRQHandler), /*LN_IRQ_ADC0_1              !< 31 ADC0 and ADC1 interrupt                                  */
VEC(unsupported), /*LN_IRQ_CAN0_TX             !< CAN0 TX interrupts                                       */
VEC(unsupported), /*LN_IRQ_CAN0_RX0            !< CAN0 RX0 interrupts                                      */
VEC(unsupported), /*LN_IRQ_CAN0_RX1            !< CAN0 RX1 interrupts                                      */
VEC(unsupported), /*LN_IRQ_CAN0_EWMC           !< CAN0 EWMC interrupts                                     */
VEC(EXTI5_9_IRQHandler), /*LN_IRQ_EXTI5_9             !< EXTI[9:5] interrupts                                     */
VEC(unsupported), /*LN_IRQ_TIMER0_BRK          !< TIMER0 break interrupts                                  */
VEC(unsupported), /*LN_IRQ_TIMER0_UP           !< TIMER0 update interrupts                                 */
VEC(unsupported), /*LN_IRQ_TIMER0_TRG_CMT      !< TIMER0 trigger and commutation interrupts                */
VEC(unsupported), /*LN_IRQ_TIMER0_Channel      !< 40 TIMER0 channel capture compare interrupts                */
VEC(unsupported), /*LN_IRQ_TIMER1              !< TIMER1 interrupt                                         */
VEC(unsupported), /*LN_IRQ_TIMER2              !< TIMER2 interrupt                                         */
VEC(unsupported), /*LN_IRQ_TIMER3              !< TIMER3 interrupts                                        */
VEC(I2C0_EV_IRQHandler), /*LN_IRQ_I2C0_EV             !< I2C0 event interrupt                                     */
VEC(I2C0_ERR_IRQHandler), /*LN_IRQ_I2C0_ER             !< I2C0 error interrupt                                     */
VEC(I2C1_EV_IRQHandler), /*LN_IRQ_I2C1_EV             !< I2C1 event interrupt                                     */
VEC(I2C1_ERR_IRQHandler), /*LN_IRQ_I2C1_ER             !< I2C1 error interrupt                                     */
VEC(unsupported), /*LN_IRQ_SPI0                !< SPI0 interrupt                                           */
VEC(unsupported), /*LN_IRQ_SPI1                !< SPI1 interrupt                                           */
VEC(USART0_IRQHandler), /*LN_IRQ_USART0              !< 50 USART0 interrupt                                         */
VEC(USART1_IRQHandler), /*LN_IRQ_USART1              !< USART1 interrupt                                         */
VEC(USART2_IRQHandler), /*LN_IRQ_USART2              !< USART2 interrupt                                         */
VEC(EXTI10_15_IRQHandler), /*LN_IRQ_EXTI10_15           !< EXTI[15:10] interrupts                                   */
VEC(RTC_IrqHandler), /*LN_IRQ_RTC_ALARM           !< RTC alarm interrupt EXTI                                 */
VEC(unsupported), /*LN_IRQ_USBFS_WKUP          !< USBFS wakeup interrupt                                   */
VEC(unsupported), /*LN_IRQ_EXMC                !< EXMC global interrupt                                    */
VEC(unsupported), /*LN_IRQ_TIMER4              !< 57 TIMER4 global interrupt                                  */
VEC(unsupported), /*LN_IRQ_SPI2                !< SPI2 global interrupt                                    */
VEC(unsupported), /*LN_IRQ_UART3               !< UART3 global interrupt                                   */
VEC(unsupported), /*LN_IRQ_UART4               !< 60 UART4 global interrupt                                   */
VEC(unsupported), /*LN_IRQ_TIMER5              !< TIMER5 global interrupt                                  */
VEC(unsupported), /*LN_IRQ_TIMER6              !< TIMER6 global interrupt                                  */
VEC(DMA1_Channel0_IRQHandler), /*LN_IRQ_DMA1_Channel0       !< DMA1 channel0 global interrupt                           */
VEC(DMA1_Channel1_IRQHandler), /*LN_IRQ_DMA1_Channel1       !< DMA1 channel1 global interrupt                           */
VEC(DMA1_Channel2_IRQHandler), /*LN_IRQ_DMA1_Channel2       !< DMA1 channel2 global interrupt                           */
VEC(DMA1_Channel3_IRQHandler), /*LN_IRQ_DMA1_Channel3       !< DMA1 channel3 global interrupt                           */
VEC(DMA1_Channel4_IRQHandler), /*LN_IRQ_DMA1_Channel4       !< DMA1 channel4 global interrupt                           */
VEC(unsupported), /*LN_IRQ_CAN1_TX             !< CAN1 TX interrupt                                        */
VEC(unsupported), /*LN_IRQ_CAN1_RX0            !< CAN1 RX0 interrupt                                       */
VEC(unsupported), /*LN_IRQ_CAN1_RX1            !< CAN1 RX1 interrupt                                       */
VEC(unsupported), /*LN_IRQ_CAN1_EWMC           !< CAN1 EWMC interrupt                                      */
VEC(unsupported), /*LN_IRQ_USBFS               !< USBFS global interrupt                                   */
};
