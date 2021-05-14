
#include "lnArduino.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"

LN_RCU *arcu=(LN_RCU *)RCU;
/**
 */
struct RCU_Peripheral
{
    Peripherals                 periph;
    int                         AHB_APB; // 1=APB 1, 2=APB2,8=AHB
    uint32_t                    enable;
};
/**
 */
static const RCU_Peripheral _peripherals[]=
{           // PERIP          APB         BIT
    {        pNONE,          0,          0},
    {        pSPI0,          2,          LN_RCU_APB2_SPI0EN},
    {        pSPI1,          1,          LN_RCU_APB1_SPI1EN},
    {        pSPI2,          1,          LN_RCU_APB1_SPI1EN},
    {        pUART0,         2,          LN_RCU_APB2_USART0EN},
    {        pUART1,         1,          LN_RCU_APB1_USART1EN},
    {        pUART2,         1,          LN_RCU_APB1_USART2EN},
    {        pUART3,         1,          LN_RCU_APB1_USART3EN},
    {        pUART4,         1,          LN_RCU_APB1_USART4EN},   
    {        pI2C0,          1,          LN_RCU_APB1_I2C0EN},   
    {        pI2C1,          1,          LN_RCU_APB1_I2C1EN},   
    {        pCAN0,          1,          LN_RCU_APB1_CAN0EN}, 
    {        pCAN1,          1,          LN_RCU_APB1_CAN1EN},  
    {        pDAC,           1,          LN_RCU_APB1_DACEN},         
    {        pPMU,           1,          LN_RCU_APB1_PMUEN},       
    {        pBKPI,          1,          LN_RCU_APB1_BKPIEN},
    {        pWWDGT,         1,          LN_RCU_APB1_WWDGTEN},
    {        pTIMER0,        2,          LN_RCU_APB2_TIMER0EN},
    {        pTIMER1,        1,          LN_RCU_APB1_TIMER1EN},
    {        pTIMER2,        1,          LN_RCU_APB1_TIMER2EN},
    {        pTIMER3,        1,          LN_RCU_APB1_TIMER3EN},
    {        pTIMER4,        1,          LN_RCU_APB1_TIMER4EN},
    {        pTIMER5,        1,          LN_RCU_APB1_TIMER5EN},
    {        pTIMER6,        1,          LN_RCU_APB1_TIMER6EN},
    {        pADC0  ,        2,          LN_RCU_APB2_ADC0EN},
    {        pADC1  ,        2,          LN_RCU_APB2_ADC1EN},
           // PERIP        AHB         APB         BIT    
    {        pGPIOA,         2,          LN_RCU_APB2_PAEN},
    {        pGPIOB,         2,          LN_RCU_APB2_PBEN},
    {        pGPIOC,         2,          LN_RCU_APB2_PCEN},
    {        pGPIOD,         2,          LN_RCU_APB2_PDEN},
    {        pGPIOE,         2,          LN_RCU_APB2_PEEN},
    {        pAF,            2,          LN_RCU_APB2_AFEN},
    {        pDMA0,          8,          LN_RCU_AHB_DMA0EN},
    {        pDMA1,          8,          LN_RCU_AHB_DMA1EN}
};

// 1 : Reset
// 2: Enable
// 3: disable
static void _rcuAction(const Peripherals periph, int action)
{
    RCU_Peripheral *o=(RCU_Peripheral *)(_peripherals+(int)periph);
    xAssert(o->periph==periph);
    xAssert(o->enable);
    switch(o->AHB_APB)
    {
        case 1: // APB1
            switch(action)
            {
                case 1: arcu->APB1RST|= o->enable;break;
                case 2: arcu->APB1EN |= o->enable;break;
                case 3: arcu->APB1EN &=~o->enable;
            }            
            break;
        case 2: // APB2
            switch(action)
            {
                case 1: arcu->APB2RST|= o->enable;break;
                case 2: arcu->APB2EN |= o->enable;break;
                case 3: arcu->APB2EN &=~o->enable;
            }            
            break;
        case 8: // AHB
            switch(action)
            {
                case 1: arcu->AHBRST|= o->enable;break;
                case 2: arcu->AHBEN |= o->enable;break;
                case 3: arcu->AHBEN &=~o->enable;
            }            

            break;
        default:
            xAssert(0);
    }
}

/**
 * 
 * @param periph
 */
void lnPeripherals::reset(const Peripherals periph)
{
    _rcuAction(periph,1);
}
/**
 * 
 * @param periph
 */
void lnPeripherals::enable(const Peripherals periph)
{
    _rcuAction(periph,2);
}
/**
 * 
 * @param periph
 */
void lnPeripherals::disable(const Peripherals periph)
{
    _rcuAction(periph,3);
}

uint32_t _rcuClockApb1,_rcuClockApb2;

/**
 * 
 * @param periph
 * @return 
 */
uint32_t lnPeripherals::getClock(const Peripherals periph)
{
    switch(periph)
    {
        case pUART0:
        case pTIMER0:
        case pSPI0:
        case pAF:
                    return _rcuClockApb2;
        default:
                    return _rcuClockApb1;
    }
}

// 
void lnInitSystemClock()
{
 
     /* reset the RCC clock configuration to the default reset state */
    /* enable IRC8M */
    RCU_CTL |= RCU_CTL_IRC8MEN;
    
    /* reset SCS, AHBPSC, APB1PSC, APB2PSC, ADCPSC, CKOUT0SEL bits */
    RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC |
                  RCU_CFG0_ADCPSC | RCU_CFG0_ADCPSC_2 | RCU_CFG0_CKOUT0SEL);

    /* reset HXTALEN, CKMEN, PLLEN bits */
    RCU_CTL &= ~(RCU_CTL_HXTALEN | RCU_CTL_CKMEN | RCU_CTL_PLLEN);

    /* Reset HXTALBPS bit */
    RCU_CTL &= ~(RCU_CTL_HXTALBPS);

    /* reset PLLSEL, PREDV0_LSB, PLLMF, USBFSPSC bits */
    
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PREDV0_LSB | RCU_CFG0_PLLMF |
                  RCU_CFG0_USBFSPSC | RCU_CFG0_PLLMF_4);
    RCU_CFG1 = 0x00000000U;

    /* Reset HXTALEN, CKMEN, PLLEN, PLL1EN and PLL2EN bits */
    RCU_CTL &= ~(RCU_CTL_PLLEN | RCU_CTL_PLL1EN | RCU_CTL_PLL2EN | RCU_CTL_CKMEN | RCU_CTL_HXTALEN);
    /* disable all interrupts */
    RCU_INT = 0x00FF0000U;
    
 //----------------------------------------------   
    uint32_t timeout   = 0U;
    uint32_t stab_flag = 0U;

    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(1){
        }
    }

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/1 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB/2 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV2;

    /* CK_PLL = (CK_PREDIV0) * 27 = 108 MHz */ 
    RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL27);

    if(HXTAL_VALUE==25000000){
		/* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 | RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PREDV1_DIV5 | RCU_PLL1_MUL8 | RCU_PREDV0_DIV10);

		/* enable PLL1 */
		RCU_CTL |= RCU_CTL_PLL1EN;
		/* wait till PLL1 is ready */
		while(0U == (RCU_CTL & RCU_CTL_PLL1STB)){
		}

		/* enable PLL1 */
		RCU_CTL |= RCU_CTL_PLL2EN;
		/* wait till PLL1 is ready */
		while(0U == (RCU_CTL & RCU_CTL_PLL2STB)){
		}
    }else if(HXTAL_VALUE==8000000){
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 | RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2 | RCU_PREDV1_DIV2 | RCU_PLL1_MUL20 | RCU_PLL2_MUL20);

		/* enable PLL1 */
		RCU_CTL |= RCU_CTL_PLL1EN;
		/* wait till PLL1 is ready */
		while(0U == (RCU_CTL & RCU_CTL_PLL1STB)){
		}

		/* enable PLL2 */
		RCU_CTL |= RCU_CTL_PLL2EN;
		/* wait till PLL1 is ready */
		while(0U == (RCU_CTL & RCU_CTL_PLL2STB)){
		}

    }
    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLL;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLL)){
    }
    
}
// EOF
