/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnPeripheral_priv.h"
#include "lnSerial.h"
#include "lnSerial_priv.h"

#pragma clang diagnostic ignored "-Wextra"

class lnSerialBp;

LN_USART_Registers *usart0 = (LN_USART_Registers *)LN_USART0_ADR;
class lnSerialBpCore;
static bool ln_serial_rawWrite(LN_USART_Registers *d, int count, unsigned char const *c);
static lnSerialBpCore *SerialInstance[5] = {NULL, NULL, NULL, NULL, NULL};
// 0   1    2    3

/**
 */
struct UsartMapping
{
    uint32_t usartEngine;
    const uint8_t dmaEngine;
    const uint8_t dmaTxChannel;
    const uint8_t dmaRxChannel;
    const uint8_t filler;
    const LnIRQ irq;
    const lnPin tx;
    const lnPin rx;
    const Peripherals periph;
};
static const UsartMapping usartMapping[3] = {
    // Adr        DMA CH IRQ         TX   RX   periperal
    {LN_USART0_ADR, 0, 3, 4, 0, LN_IRQ_USART0, PA9, PA10, pUART0},
    {LN_USART1_ADR, 0, 6, 5, 0, LN_IRQ_USART1, PA2, PA3, pUART1},
    {LN_USART2_ADR, 0, 1, 2, 0, LN_IRQ_USART2, PB10, PB11, pUART2},
};
/**
 *
 * @param instance
 * @param irq
 */
#define M(x) usartMapping[instance].x

/**
 */
#include "lnDma.h"
/**
 * @brief
 *
 */
class lnSerialBpCore : public lnSerialCore
{
  public:
    enum txState
    {
        txTransmittingIdle,
        txTransmittingInterrupt,
        txTransmittingDMA,
        txTransmittingLast
    };

    // public API
    lnSerialBpCore(int instance);
    bool init();
    bool setSpeed(int speed);

    // implementation
    virtual void _interrupt(void) = 0;

    static void interrupts(int instance);

  protected:
    virtual bool _programTx() = 0;
    void disableInterrupt();
    void enableInterrupt(bool tx, bool rx);
    bool _enableTx(txState mode);
    LnIRQ _irq;
    uint32_t _adr;
    // tx
    volatile const uint8_t *_cur, *_tail;
    txState _txState;
    xBinarySemaphore _txDone;
    xMutex _txMutex;
};
/**
 * @brief Construct a new ln Serial::ln Serial object
 *
 * @param instance
 * @param rxBufferSize
 */
lnSerialBpCore::lnSerialBpCore(int instance) : lnSerialCore(instance)
{
    const UsartMapping *m = usartMapping + instance;
    _instance = instance;
    _irq = m->irq;
    _adr = m->usartEngine;
    xAssert(SerialInstance[instance] == NULL);
    SerialInstance[instance] = this;
    _txState = txTransmittingIdle;
}

void lnSerialBpCore::enableInterrupt(bool tx, bool rx)
{
    if (tx || rx)
        lnEnableInterrupt(_irq);
    else
        lnDisableInterrupt(_irq);
}

/**
 *
 * @param speed
 * @return
 */
bool lnSerialBpCore::setSpeed(int speed)
{
    int freq;

    Peripherals periph = (Peripherals)((int)pUART0 + _instance);
    freq = lnPeripherals::getClock(periph);
    // compute closest divider
    int divider = (freq + (speed / 2)) / speed;
    // Fixed 4 buts decimal, just ignore since we oversample by 16
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    d->CTL0 &= ~LN_USART_CTL0_UEN;
    d->BAUD = divider; // change speed when usart is off
    d->CTL0 |= LN_USART_CTL0_UEN;
    return true;
}
/**
 *
 * @return
 */
bool lnSerialBpCore::init()
{
    LN_USART_Registers *d = (LN_USART_Registers *)_adr;
    const UsartMapping *e = usartMapping + _instance;

    switch (_instance)
    {
    case 0:
    case 1:
    case 2:
        lnPinMode(e->tx, lnALTERNATE_PP);
        // ? lnPinMode(e->rx,lnALTERNATE_OD);
        lnPeripherals::enable(e->periph);
        break;
    default:
        xAssert(0);
        break;
    }
    // Disable RX & TX
    d->CTL0 &= ~LN_USART_CTL0_KEEP_MASK; // default is good enough
    setSpeed(115200);
    d->CTL0 |= LN_USART_CTL0_UEN; // enable uart
    return true;
}

/**

*/
void lnSerialBpCore::disableInterrupt()
{
    const UsartMapping *m = usartMapping + _instance;
    lnDisableInterrupt(m->irq);
}
/**

*/
void lnSerialBpCore::interrupts(int instance)
{
    lnSerialBpCore *inst = SerialInstance[instance];
    xAssert(inst);
    inst->_interrupt();
}

#define IRQHANDLER(x)                                                                                                  \
    extern "C"                                                                                                         \
    {                                                                                                                  \
        void USART##x##_IRQHandler() LN_INTERRUPT_TYPE;                                                                \
        void USART##x##_IRQHandler()                                                                                   \
        {                                                                                                              \
            lnSerialBpCore::interrupts(x);                                                                             \
        }                                                                                                              \
    }

IRQHANDLER(0)
IRQHANDLER(1)
IRQHANDLER(2)
IRQHANDLER(3)

#include "lnSerialTxOnly.cpp"
#include "lnSerialTxOnlyDma.cpp"
#include "lnSerialTxOnlyDmaBuffer.cpp"
#include "lnSerialRxTx.cpp"
#include "lnSerialRxTxDma.cpp"
/**
 * @brief Create a Ln Serial Tx Only object
 *
 * @param instance
 * @param dma
 * @return lnSerialTxOnly*
 */
lnSerialTxOnly *createLnSerialTxOnly(int instance, bool dma, bool buffered)
{
    if (dma)
    {
        if (buffered)
        {
            return new lnSerialBpTxOnlyBufferedDma(instance, 128);
        }
        else
        {
            return new lnSerialBpTxOnlyDma(instance);
        }
    }
    return new lnSerialBpTxOnlyInterrupt(instance);
}
/**
 * @brief Create a Ln Serial Rx Tx object
 *
 * @param instance
 * @param rxBufferSize
 * @param dma
 * @return lnSerialRxTx*
 */
lnSerialRxTx *createLnSerialRxTx(int instance, int rxBufferSize, bool dma)
{
    if (dma)
        return new lnSerialBpRxTxDma(instance, rxBufferSize);
    return new lnSerialBpRxTx(instance, rxBufferSize);
}
/**
 * @brief
 *
 * @param count
 * @param c
 * @return true
 * @return false
 */
bool ln_serial_rawWrite(LN_USART_Registers *d, int count, unsigned char const *c)
{
    while (count--)
    {
        const char a = *c++;
        volatile uint32_t stat = d->STAT;
        while (!(stat & (LN_USART_STAT_TBE)))
        {
            stat = d->STAT;
        }
        d->DATA = a;
    }
    return true;
}
// EOF
